#include "kernel.h"

using namespace Memory;

void *__gxx_personality_v0;
void *_Unwind_Resume;
void *__cxa_atexit;
void *__dso_handle;

Point MainTextCursor;
IDTR idtr;
BasicRenderer basicRenderer(NULL, NULL); //_start will init it
BootInfo *bootInfo;
KernelInfo *kernelInfo;

extern "C" void _start(BootInfo *_bootInfo)
{
    bootInfo = _bootInfo;

    basicRenderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_font);
    basicRenderer.Logln("Kernel boot process...");

    /*Global Descriptor Table*/
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);
    
    /*Paging - PageTable - Kernel Size*/
    uint64_t MemoryMapEntries = bootInfo->mMapSize / bootInfo->mMapDescriptorSize;

    GlobalAllocator = PageFrameAllocator();
    GlobalAllocator.ReadEfiMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescriptorSize, &basicRenderer);
    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
    GlobalAllocator.LockPages(&_KernelStart, kernelPages);

    uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;
    GlobalAllocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);
    
    GlobalAllocator.LockPages(&_KernelStart, kernelPages);

    /*Showing memory infos on screen*/
    KernelShowBootInfos(bootInfo, kernelSize, MemoryMapEntries);

    /*Preparing interrupt*/
    basicRenderer.Log("Initializing interrupts...");
    idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64_t)GlobalAllocator.RequestPage();

    SetIDTGate((void*)PageFaultHandler, 0xE, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)DoubleFaultHandler, 0x8, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)GeneralProtectionHandler, 0xD, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)KeyboardInterruptHandler, 0x21, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)MouseInterruptHandler, 0x2C, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)PITHandler, 0x20, IDT_TA_InterruptGate, 0x08);

    asm ("lidt %0" : : "m" (idtr));
    basicRenderer.OK();

    basicRenderer.Log("Remaping PIC...");
    RemapPIC();
    basicRenderer.OK();

    basicRenderer.Log("Initializing PS2 mouse...");
    // Mouse mouse;
    // InitPS2Mouse();
    mouse.init();
    basicRenderer.OK();

    basicRenderer.Logln("Masking Interrupts...");
    outb(PIC1_DATA, 0b11111000);
    outb(PIC2_DATA, 0b11101111);
    asm("sti");
    
    Keyboard keyboard = {QWERTY_TYPE};
    basicRenderer.Logln("PS2 Keyboard initialized!");
    
    uint64_t StartMemoryManager = (uint64_t)GlobalAllocator.RequestPage();
    uint64_t LengthMemoryAvailableForAllocator = GetMemorySize(bootInfo->mMap, MemoryMapEntries, bootInfo->mMapDescriptorSize) - GlobalAllocator.GetReservedMemory() - GlobalAllocator.GetUsedMemory() - 4096;

    memorymanager.init(StartMemoryManager, LengthMemoryAvailableForAllocator);
    GlobalAllocator.LockPages((void*)StartMemoryManager, (uint64_t)LengthMemoryAvailableForAllocator / 4096 + 1);
    
    if(!memorymanager.test())
    {
        panic("Error while doing dynamic allocation test!");
        while(1);
    }
    basicRenderer.Logln("Talking with PCI...");
    basicRenderer.Logln("Devices detected : ");
    PCI pci;
    DriversLoader dloader;
    for(uint8_t bus = 0; bus < 8; bus++)
    {
        for(uint8_t device = 0; device < 32; device++)
        {
            for(uint8_t function = 0; function < 8; function++)
            {
                uint16_t tmp = pci.GetVendor(bus, device, function);
                if(tmp != 0xFFFF)
                {
                    uint16_t Vendor = tmp;
                    uint16_t Device = pci.GetDevice(bus, device, function);
                    uint8_t Class = pci.GetClass(bus, device, function);
                    uint8_t SubClass = pci.GetSubClass(bus, device, function);
                    basicRenderer.Print(pci.GetVendorName(tmp));
                    basicRenderer.putChar('/');
                    basicRenderer.Print(pci.GetDeviceName(Vendor, Device));
                    basicRenderer.putChar('/');
                    basicRenderer.Print(pci.GetClassName(Class));
                    basicRenderer.putChar('/');
                    basicRenderer.Print(pci.GetSubClassName(Class, SubClass));
                    uint8_t type = pci.CheckHeaderType(bus, device, function);
                    if(type == PCI_GENERAL_DEVICE) basicRenderer.Print(" => General Device");
                    else if(type == PCI_TO_PCI_BRIDGE) basicRenderer.Print(" => PCI to PCI bridge");
                    else if(type == PCI_CARDBUS) basicRenderer.Print(" => PCI Cardbus");
                    else basicRenderer.Print(" => Unknown type");
                    basicRenderer.Print(" 0x");
                    uint32_t bar = pci.get_bar(bus, device, function, 0);
                    basicRenderer.Print(ToHexString((uint32_t)bar));
                    dloader.save(Vendor, Device, Class);
                }
            }
        }
    }
    
    /*ACPI*/
    ACPI acpi((RSDPDescriptorNewVersion*)bootInfo->rsdp);

    dump_registers();

    PIT.init(1000);

    while(true)
    {
        // ProcessMousePacket();
        mouse.main();
    }
    while(true);
}

void KernelShowBootInfos(BootInfo *bootInfo, uint64_t kernelSize, uint64_t MemoryMapEntries)
{
    basicRenderer.clear();
    basicRenderer.Log("Base address of framebuffer : 0x");
    basicRenderer.Print(ToHexString((uint64_t)bootInfo->framebuffer->BaseAddress));
    basicRenderer.OK();
    basicRenderer.Log("Number of pixels on width : ");
    basicRenderer.Print(ToString((uint64_t)bootInfo->framebuffer->Width));
    basicRenderer.OK();
    basicRenderer.Log("Number of pixels on height : ");
    basicRenderer.Print(ToString((uint64_t)bootInfo->framebuffer->Height));
    basicRenderer.OK();
    basicRenderer.Log("Memory size : ");
    basicRenderer.Print(ToString((double)GetMemorySize(bootInfo->mMap, MemoryMapEntries, bootInfo->mMapDescriptorSize) / 1024 / 1024));
    basicRenderer.Println(" MB.");
    basicRenderer.Log("Kernel size : ");
    basicRenderer.Print(ToString((double)kernelSize / 1024 / 1024));
    basicRenderer.Println(" MB.");
    basicRenderer.Log("Free memory : ");
    basicRenderer.Print(ToString((double)GlobalAllocator.GetFreeMemory() / 1024 / 1024));
    basicRenderer.Println(" MB.");
    basicRenderer.Log("Used memory : ");
    basicRenderer.Print(ToString((double)GlobalAllocator.GetUsedMemory() / 1024 / 1024));
    basicRenderer.Println(" MB.");
    basicRenderer.Log("Used memory (%) : ");
    basicRenderer.Print(ToString(GlobalAllocator.GetPercentUsedMemory()));
    basicRenderer.Println(" %.");
    basicRenderer.Log("Reserved memory : ");
    basicRenderer.Print(ToString((double)GlobalAllocator.GetReservedMemory() / 1024 / 1024));
    basicRenderer.Println(" MB.");
    // basicRenderer.Log("Paging initialized!");
    // basicRenderer.OK();
    basicRenderer.Log("Global Descriptor Table initialized!");
    basicRenderer.OK();
}

/*IDTDescriptorEntry *IntPageFault =(IDTDescriptorEntry*)(idtr.Offset + 0x0E * sizeof(IDTDescriptorEntry)); //page fault
IntPageFault->SetOffset((uint64_t)PageFaultHandler);
IntPageFault->type_attr = IDT_TA_InterruptGate;
IntPageFault->selector = 0x08;*/

/*PageTable *PML4 = (PageTable*)GlobalAllocator.RequestPage();
memset(PML4, 0, 0x1000);
    
PageTableManager pageTableManager = PageTableManager(PML4);
for(uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, MemoryMapEntries, bootInfo->mMapDescriptorSize); t += 0x1000) pageTableManager.MapMemory((void*)t, (void*)t);*/

/*Mapping memory*/
/*  for(uint64_t t = fbBase; t < fbBase + fbSize; t += 4096)
    {
        pageTableManager.MapMemory((void*)t, (void*)t);
    }
    asm ("mov %0, %%cr3" : : "r"(PML4));

    memset(bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize);*/
