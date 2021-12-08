#pragma once

#include <stdint.h>

struct EFI_MEMORY_DESCRIPTOR
{
    uint32_t type;
    void *physicalAddress;
    void *virtAddress;
    uint64_t NumberPages;
    uint64_t Attr;
};

extern const char *EFI_MEMORY_TYPE_STRING[];