#include "paging.h"

void PageDirectoryEntry::SetFlag(PT_Flags flag, bool enabled)
{
    uint64_t bitSelector = (uint64_t)1 << flag;
    value &= ~bitSelector;
    if(enabled)
    {
        value |= bitSelector;
    }
}

bool PageDirectoryEntry::GetFlag(PT_Flags flag)
{
    uint64_t bitSelector = (uint64_t)1 << flag;
    return value & bitSelector > 0 ? true : false;
}

uint64_t PageDirectoryEntry::GetAddress()
{
    return (value & 0x000FFFFFFFFFF000) >> 12;
}

void PageDirectoryEntry::SetAddress(uint64_t address)
{
    address &= 0x000000FFFFFFFFFF;
    value &= 0xFFF0000000000FFF;
    value |= (address << 12);
}