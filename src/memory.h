#pragma once

#include <stdint.h>
#include "memory/efiMemory.h"
#include "kernel.h"

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR *Map, uint64_t MapEntries, uint64_t MapDescriptorSize);
void memset(void *start, uint8_t value, uint64_t num);
void memcpy(void *dest, const void * src, size_t n);
bool memcmp(const char *a, const char *b, uint8_t length);