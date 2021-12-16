#include "cpu.h"

CPU::CPU()
{
    detect_cpuid();
    basicRenderer.Log("EFLAGS value detected : 0x");
    basicRenderer.Print(ToHexString((uint64_t)store_result));
    basicRenderer.Println("!");
    if(store_result)
    {
        this->cpuid_supported = true;
        basicRenderer.Logln("CPUID detected!");
        this->vendorid();
    }
    else
    {
        this->cpuid_supported = false;
        basicRenderer.Logln("CPUID is not present!");
    }
}

CPU::~CPU(){}

void CPU::vendorid()
{
    this->cpuid(0);
    memcpy(this->vendor_id_ptr, (char*)this->cpuid(0), 12);
    basicRenderer.Log("CPU vendor detected as : ");
    basicRenderer.Println((char*)this->vendor_id_ptr);
    switch(*(char*)this->vendor_id_ptr)
    {
        case *(char*)CPUID_VENDOR_AMD:
            basicRenderer.Logln("AMD CPU detected!");
            this->cpu_type = AMD;
            break;
        case *(char*)CPUID_VENDOR_INTEL:
            basicRenderer.Logln("INTEL CPU detected!");
            this->cpu_type = INTEL;
            break;
        default:
            basicRenderer.Errorln("Unknown cpu type!");
            this->cpu_type = UNKNOWN;
            break;
    }
}

CPUID_COMMAND_OUTPUT *CPU::cpuid(uint64_t command)
{
    return (CPUID_COMMAND_OUTPUT*)cpuid_command(command);
}