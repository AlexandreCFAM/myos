#pragma once

#include "chunk.h"
#include <stdint.h>
#include "../kernel.h"

namespace Memory
{
    class Manager
    {
    public:
        Manager();
        void init(uint64_t start, uint64_t size);
        void *malloc(uint64_t size);
        void free(void *ptr);
        bool test();
    private:
        Memory::chunk *first;
        uint64_t endmemory;
    };
};

extern Memory::Manager memorymanager;