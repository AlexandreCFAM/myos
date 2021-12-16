#ifndef __CPU__H
#define __CPU__H

#include <stdint.h>
#include "../kernel.h"
#include "vendors.h"
#include "../memory.h"

struct CPUID_COMMAND_OUTPUT
{
    uint64_t rbx;
    uint64_t rdx;
    uint64_t rcx;
}__attribute__((packed));

enum CPU_TYPE
{
    INTEL,
    AMD,
    UNKNOWN
};

class CPU
{
public:
    CPU();
    ~CPU();
    void vendorid();
private:
    bool cpuid_supported;
    char *vendor_id_ptr[12];
    CPUID_COMMAND_OUTPUT *cpuid(uint64_t command);
    CPU_TYPE cpu_type;
};

extern CPU cpu;
extern "C" void detect_cpuid();
extern uint32_t store_result;
extern "C" void read_vendor_id();
extern char *vendor_id[12];
extern "C" uint64_t *cpuid_command(uint64_t arg);

#endif