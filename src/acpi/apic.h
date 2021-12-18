#ifndef __APIC__H
#define __APIC__H

#include <stdint.h>
#include "acpi.h"
#include "../interrupts/pic.h"

struct APIC_HEADER
{
    SDTCommonHeader header;
    uint32_t *apic_local_address;
    uint32_t flags;
    uint8_t entry_type;
    uint8_t record_length;
}__attribute__((packed));

class APIC
{
public:
    APIC();
    ~APIC();
    void init(SDTCommonHeader *apic);
private:
    APIC_HEADER *whereami;
};

extern APIC apic_class;

#endif