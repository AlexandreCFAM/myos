#ifndef __CPU__H
#define __CPU__H

#include <stdint.h>
#include "../kernel.h"
#include "vendors.h"
#include "../memory.h"

class CPU
{
public:
    CPU();
    ~CPU();
    void vendorid();
    static inline void cpuid()
private:
    bool cpuid_supported;
    char *vendor_id_ptr[12];
};

extern CPU cpu;
extern "C" void detect_cpuid();
extern uint32_t store_result;
extern "C" void read_vendor_id();
extern char *vendor_id[12];

#endif