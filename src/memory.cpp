#include "memory.h"

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR *Map, uint64_t MapEntries, uint64_t MapDescriptorSize)
{
    static uint64_t MemorySizeInBytes = 0;
    if(MemorySizeInBytes) return MemorySizeInBytes;
    else
    {
        for(int i = 0; i < MapEntries; i++)
        {
            EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)Map + (i * MapDescriptorSize));
            MemorySizeInBytes += desc->NumberPages * 4096;
        }
        return MemorySizeInBytes;
    }
}

void memset(void* start, uint8_t value, uint64_t num){
    for (uint64_t i = 0; i < num; i++){
        *(uint8_t*)((uint64_t)start + i) = value;
    }
}

void memcpy(void *dest, const void * src, size_t n)
{
    for(size_t i = 0; i < n; i++)
    {
        *(char*)((char*)dest + i) = *(char*)((char*)src + i);
    }
}

bool memcmp(const char *a, const char *b, uint8_t length)
{
    a = (char*)a;
    b = (char*)b;
    for(uint8_t i = 0; i < length; i++)
    {
        if(*a != *b) return false;
        else
        {
            a++;
            b++;
        }
    }
    return true;
}