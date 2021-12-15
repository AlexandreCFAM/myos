#pragma once

#include "../lib/defs.h"
#include <stdint.h>

namespace Memory
{
    struct chunk
    {
        Memory::chunk *next;
        Memory::chunk *previous;
        bool allocated;
        uint64_t size;
    }__attribute__((packed));
};