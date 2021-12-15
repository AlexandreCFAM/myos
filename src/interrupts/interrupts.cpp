#include "interrupts.h"

__attribute__((interrupt)) void PageFaultHandler(struct interrupt_frame *frame)
{
    panic("Page Fault Detected!");
    while(1);
}

__attribute__((interrupt)) void DoubleFaultHandler(struct interrupt_frame *frame)
{
    panic("Double Fault Detected!");
    while(1);
}
__attribute__((interrupt)) void GeneralProtectionHandler(struct interrupt_frame *frame)
{
    panic("General Protection Fault!");
    while(1);
}

__attribute__((interrupt)) void KeyboardInterruptHandler(struct interrupt_frame *frame)
{
    uint8_t scanCode = inb(0x60);
    keyboard.handle(scanCode);
    PIC_EndMaster();
}

__attribute__((interrupt)) void MouseInterruptHandler(struct interrupt_frame *frame)
{
    uint8_t mouseData = inb(0x60);
    HandlePS2Mouse(mouseData);
    PIC_EndSlave();
}

__attribute__((interrupt)) void PITHandler(struct interrupt_frame *frame)
{
    PIT.Tick++;
    // basicRenderer.putChar('l');
    PIC_EndMaster();
}
