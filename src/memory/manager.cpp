#include "manager.h"

static Memory::Manager *active;

namespace Memory
{

    Manager::Manager(uint64_t start, uint64_t size)
    {
        basicRenderer.Logln("Initializing memory manager for dynamic allocation...");
        Manager::first = (Memory::chunk*)start;
        if(size < sizeof(Memory::chunk))
        {
            panic("Memory Manager : size < sizeof a chunk!");
            while(1);
        }
        basicRenderer.Log("Memory Manager will allocate from 0x");
        basicRenderer.Print(ToHexString(start));
        basicRenderer.Print(" to 0x");
        basicRenderer.Print(ToHexString(start + size));
        basicRenderer.OK();

        Manager::first->size = size - sizeof(Memory::chunk);
        Manager::first->allocated = false;
        Manager::first->next = 0;
        Manager::first->previous = 0;

        basicRenderer.Logln("First chunk node initialized!");
        endmemory = start + size;
    }

    void *Manager::malloc(uint64_t size)
    {
        Memory::chunk *result = NULL;
        for(Memory::chunk *chunk = first; chunk != 0; chunk = chunk->next)
        {
            if(chunk->size >= size + sizeof(Memory::chunk) + 1 && !chunk->allocated)
            {
                result = chunk;
                break;
            }
        }
        if(result == 0) return 0;
        else
        {
            result->allocated = true;
            Memory::chunk *temp = (Memory::chunk*)((uint64_t)result + size + sizeof(Memory::chunk));
            temp->allocated = false;
            temp->previous = result;
            temp->size = result->size - size - sizeof(Memory::chunk);
            temp->next = result->next;
            result->next = temp;
            result->size = size;
            return (void*)((uint64_t)result + sizeof(Memory::chunk));
        }
        return 0;
    }

    void Manager::free(void *ptr)
    {
        Memory::chunk *chunkToRemove = (Memory::chunk*)((uint64_t)ptr - sizeof(Memory::chunk));

        chunkToRemove->allocated = false;

        //if there is a white chunk in previous we can add them
        if(chunkToRemove->previous != 0 && !chunkToRemove->previous->allocated)
        {
            chunkToRemove->previous->next = chunkToRemove->next;
            chunkToRemove->previous->size += sizeof(Memory::chunk) + chunkToRemove->size;
        }
        //the same for the next
        if(chunkToRemove->next != 0 && !chunkToRemove->next->allocated)
        {
            chunkToRemove->next->previous = chunkToRemove->previous;
            chunkToRemove->size += sizeof(Memory::chunk) + chunkToRemove->next->size;
        }
    }

    bool Manager::test()
    {
        basicRenderer.Logln("Testing dynamic allocation ...");
        basicRenderer.Log("Allocating 8 bytes... : 0x");
        uint64_t *ptr = (uint64_t*)Manager::malloc(8);
        basicRenderer.Print(ToHexString((uint64_t)ptr));
        if(ptr == 0)
        {
            basicRenderer.ErrorFlag();
            return false;
        }
        else basicRenderer.OK();
        basicRenderer.Logln("Using test value (191004) with pointer...");
        *ptr = 191004;
        basicRenderer.Log("Value read : ");
        basicRenderer.Print(ToString((uint64_t)*ptr));
        if(*ptr == 191004) basicRenderer.OK();
        else
        {
            basicRenderer.ErrorFlag();
            return false;
        }
        basicRenderer.Log("Testing free function...");
        Manager::free((void*)ptr);
        basicRenderer.OK();
        return true;
    }
};