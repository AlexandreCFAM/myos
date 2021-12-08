#ifndef __CPU_REGISTERS__H
#define __CPU_REGISTERS__H

#include <stdint.h>

void dump_registers();
extern "C" void dump_registers_asm();

struct cpu_registers
{
    uint64_t rax;
    uint64_t rbx;
}__attribute__((packed));

extern cpu_registers entry_point;
extern uint8_t number_of_registers;

#endif