#pragma once

#include "../kernel.h"
#include "../panic.h"
#include "../IO.h"
#include "../drivers/keyboard.h"
#include "../drivers/PS2/mouse.h"

#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

struct interrupt_frame;
__attribute__((interrupt)) void PageFaultHandler(struct interrupt_frame *frame);
__attribute__((interrupt)) void DoubleFaultHandler(struct interrupt_frame *frame);
__attribute__((interrupt)) void GeneralProtectionHandler(struct interrupt_frame *frame);
__attribute__((interrupt)) void KeyboardInterruptHandler(struct interrupt_frame *frame);
__attribute__((interrupt)) void MouseInterruptHandler(struct interrupt_frame *frame);

void RemapPIC();
void PIC_EndMaster();
void PIC_EndSlave();