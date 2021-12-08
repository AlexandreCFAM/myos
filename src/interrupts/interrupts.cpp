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
    HandleKeyboard(scanCode);
    PIC_EndMaster();
}

void RemapPIC()
{
    uint8_t a1, a2;
    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();
    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();
    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();
    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();
    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);
    io_wait();
}

void PIC_EndMaster()
{
    outb(PIC1_CMD, PIC_EOI);
}

void PIC_EndSlave()
{
    outb(PIC2_CMD, PIC_EOI);
    outb(PIC1_CMD, PIC_EOI);
}

__attribute__((interrupt)) void MouseInterruptHandler(struct interrupt_frame *frame)
{
    uint8_t mouseData = inb(0x60);
    HandlePS2Mouse(mouseData);
    PIC_EndSlave();
}