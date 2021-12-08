#include "efiMemory.h"

const char *EFI_MEMORY_TYPE_STRING[] = {
    "EfiReservedMemoryType",
    "EfiLoaderCode",
    "EfiLoaderData",
    "EfiBootServicesCode",
    "EfiBootServicesData",
    "EfiRuntimeServicesCode",
    "EfiRuntimeServicesData",
    "EfiConventionalMemory",
    "EfiUnusableMemory",
    "EfiACPIReclaimMemory",
    "EfiACPIMemoryNVS",
    "EdiMemoryMappedIO",
    "EfiMemoryMappedIOPortSpace", 
    "EfiPalCode",
};