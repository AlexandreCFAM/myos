#pragma once

#include <stdint.h>

enum PT_Flags
{
    Present = 0,
    ReadWrite = 1,
    UserSuper = 2,
    WriteThrough = 3,
    CacheDisabled = 4,
    Accessed = 5,
    LargerPages = 7,
    Custom0 = 9,
    Custom1 = 10,
    Curstom2 = 11,
    NX = 63 //only if supported
};

struct PageDirectoryEntry {
    uint64_t value;
    void SetFlag(PT_Flags flag, bool enables);
    bool GetFlag(PT_Flags flag);
    void SetAddress(uint64_t address);
    uint64_t GetAddress();
};

struct PageTable { 
    PageDirectoryEntry entries [512];
}__attribute__((aligned(0x1000)));