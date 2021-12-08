#include "PageFrameAllocator.h"
#include "../memory/bitmap.h"
#include "../memory/efiMemory.h"
#include "../memory.h"
#include "../defs.h"
#include "../kernel.h"

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
bool Initialized = false;

PageFrameAllocator GlobalAllocator;

uint64_t pageBitmapIndex = 0;


void PageFrameAllocator::ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR *MemoryMap, size_t MapSize, size_t MapDescriptorSize, BasicRenderer *renderer)
{
    if(Initialized) return;
    uint64_t MemoryMapEntries = MapSize / MapDescriptorSize;
    void *largestFreeMemorySegment = NULL;
    size_t largestFreeMemorySegmentSize;
    for(int i = 0; i < MemoryMapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)MemoryMap + (i * MapDescriptorSize));
        if(desc->type == 7)
        {
            //EfiConventionalMemory
            if(desc->NumberPages * 4096 > largestFreeMemorySegmentSize)
            {
                largestFreeMemorySegment = desc->physicalAddress;
                largestFreeMemorySegmentSize = desc->NumberPages * 4096;
            }
        }
    }
    uint64_t memorySize = GetMemorySize(MemoryMap, MemoryMapEntries, MapDescriptorSize);
    freeMemory = memorySize;
    uint64_t bitmapSize = memorySize / 4096 / 8 + 1;
    //Initialize bitmap
    PageFrameAllocator::InitBitmap(bitmapSize, largestFreeMemorySegment);
    LockPages(&PageBitmap.Buffer, PageBitmap.Size / 4096 + 1);
    for(int i = 0; i < MemoryMapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)MemoryMap + (i * MapDescriptorSize));
        if(desc->type != 7)
        {
            ReservePages(desc->physicalAddress, desc->NumberPages);
        }
    }
    Initialized = true;
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void *bufferAddress)
{
    PageFrameAllocator::PageBitmap.Size = bitmapSize;
    PageFrameAllocator::PageBitmap.Buffer = (uint8_t*)bufferAddress;
    for(int i = 0; i < bitmapSize; i++)
    {
        *(uint8_t*)(PageFrameAllocator::PageBitmap.Buffer + i) = 0;
    }
}

void PageFrameAllocator::FreePage(void *address)
{
    uint64_t index = (uint64_t)address / 4096;
    if(PageBitmap[index] == false) return; //already free
    else
    {
        if(PageBitmap.Set(index, false))
        {
            freeMemory += 4096;
            usedMemory -= 4096;
            if(pageBitmapIndex > index)
            {
                pageBitmapIndex = index;
            }
        }
    }
}

void PageFrameAllocator::FreePages(void *address, uint64_t pageCount)
{
    for(int t = 0; t < pageCount; t++)
    {
        FreePage((void*)((uint64_t)address + (t * 4096)));
    }
}

void PageFrameAllocator::LockPages(void *address, uint64_t pageCount)
{
    for(int t = 0; t < pageCount; t++)
    {
        LockPage((void*)((uint64_t)address + (t * 4096)));
    }
}

void PageFrameAllocator::ReservePages(void *address, uint64_t pageCount)
{
    for(int t = 0; t < pageCount; t++)
    {
        ReservePage((void*)((uint64_t)address + (t * 4096)));
    }
}

void PageFrameAllocator::UnreservePages(void *address, uint64_t pageCount)
{
    for(int t = 0; t < pageCount; t++)
    {
        UnreservePage((void*)((uint64_t)address + (t * 4096)));
    }
}

void PageFrameAllocator::LockPage(void *address)
{
    uint64_t index = (uint64_t)address / 4096;
    if(PageBitmap[index] == true) return; //already locked
    else
    {
        if(PageBitmap.Set(index, true))
        {
            freeMemory -= 4096;
            usedMemory += 4096;
        }
    }
}

void PageFrameAllocator::UnreservePage(void *address)
{
    uint64_t index = (uint64_t)address / 4096;
    if(PageBitmap[index] == false) return; //already free
    else
    {
        if(PageBitmap.Set(index, false))
        {
            freeMemory += 4096;
            reservedMemory -= 4096;
            if(pageBitmapIndex > index)
            {
                pageBitmapIndex = index;
            }
        }
    }
}

void PageFrameAllocator::ReservePage(void *address)
{
    uint64_t index = (uint64_t)address / 4096;
    if(PageBitmap[index] == true) return; //already locked
    else
    {
        if(PageBitmap.Set(index, true))
        {
            freeMemory -= 4096;
            reservedMemory += 4096;
        }
    }
}

uint64_t PageFrameAllocator::GetFreeMemory()
{
    return freeMemory;
}

uint64_t PageFrameAllocator::GetUsedMemory()
{
    return usedMemory;
}

uint64_t PageFrameAllocator::GetReservedMemory()
{
    return reservedMemory;
}

void *PageFrameAllocator::RequestPage()
{
    for(;pageBitmapIndex < PageBitmap.Size * 8; pageBitmapIndex++)
    {
        if(PageBitmap[pageBitmapIndex] == true) continue;
        else
        {
            LockPage((void*)(pageBitmapIndex * 4096));
            return (void*)(pageBitmapIndex * 4096);
        }
    }
    return NULL; //todo swap
}

double PageFrameAllocator::GetPercentUsedMemory()
{
    return (usedMemory / (double)GetMemorySize(bootInfo->mMap, kernelInfo->MemoryMapEntries, bootInfo->mMapDescriptorSize)) * 100;
}