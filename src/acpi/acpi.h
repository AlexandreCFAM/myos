#ifndef __ACPI__H
#define __ACPI__H

#include "rsdp.h"
#include "../panic.h"
#include "sdt.h"
#include "fadt.h"
#include "apic.h"

class ACPI
{
public:
    ACPI(struct RSDPDescriptorNewVersion* rsdp);
    ~ACPI();
    bool IsNewVersionACPI();
    bool ValidateRSDPFirstPart();
    uint64_t GetValueValidateRSDPFirstPart();
    uint64_t GetValueValidateRSDPSecondPart();
    bool CheckSignature(SDTCommonHeader *table, char *signature);
    unsigned char Checksum(SDTCommonHeader *table);
    bool IsChecksumCorrect(SDTCommonHeader *table);
    SDTCommonHeader *FindTable(char *signature);
    void ListTables();
private:
    RSDPDescriptorNewVersion *_rsdp;
    bool version;
    bool validate;
    struct XSDT *xsdt_ptr;
};

struct SystemConfiguration
{
    uint8_t PowerManagementProfile;
    XSDT *xsdt;
    FADT *fadt;
    SDTCommonHeader *apic;
};

extern SystemConfiguration MainSystemConfiguration;

#endif