#pragma once

#include "../kernel.h"
#include "../panic.h"
#include "../hardware/IO.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/PS2/mouse.h"
#include "pic.h"


struct interrupt_frame;
__attribute__((interrupt)) void PageFaultHandler(struct interrupt_frame *frame);
__attribute__((interrupt)) void DoubleFaultHandler(struct interrupt_frame *frame);
__attribute__((interrupt)) void GeneralProtectionHandler(struct interrupt_frame *frame);
__attribute__((interrupt)) void KeyboardInterruptHandler(struct interrupt_frame *frame);
__attribute__((interrupt)) void MouseInterruptHandler(struct interrupt_frame *frame);
__attribute__((interrupt)) void PITHandler(struct interrupt_frame *frame);