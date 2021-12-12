#include "registers.h"
#include "../kernel.h"

const char* cpu_registers_name[] = {
    "RAX",
    "RBX",
    "RCX",
    "RDX",
    "RDI",
    "RSI",
    "RBP",
    "R8",
    "R9",
    "R10",
    "R11",
    "R12",
    "R13",
    "R14",
    "R15"
};

void dump_registers()
{
    basicRenderer.Logln("Saving registers state in memory...");
    dump_registers_asm();
    basicRenderer.Logln("Here are the registers : ");
    uint64_t *ptr = (uint64_t*)&entry_point;
    for(int i = 0; i < number_of_registers; i++)
    {
        basicRenderer.Print(cpu_registers_name[i], YELLOW);
        basicRenderer.Print(" => 0x", YELLOW);
        basicRenderer.Println(ToHexString((uint64_t)*ptr), YELLOW);
        ptr++;
    }
}