#ifndef __FADT__H
#define __FADT__H

#include "acpi.h"
#include "../kernel.h"

class FixedACPIDescriptionTable
{
public:
    FixedACPIDescriptionTable(FADT *fadt);
    ~FixedACPIDescriptionTable();
    void main();
    void powermanagement();
private:
    FADT *fadt;
};

#endif