#pragma once

#include "../memory/efiMemory.h"
#include <stdint.h>
#include "../memory/bitmap.h"
#include "../memory.h"
#include "../lib/defs.h"
#include "../graphics/textrenderer.h"
#include "../kernel.h"

class PageFrameAllocator
{
public:
    void ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR *MemoryMap, size_t MapSize, size_t MapDescriptorSize, TextRenderer *renderer);
    Bitmap PageBitmap;
    void FreePage(void *address);
    void LockPage(void *address);
    void FreePages(void *address, uint64_t pageCount);
    void LockPages(void *address, uint64_t pageCount);
    void *RequestPage();
    uint64_t GetFreeMemory();
    uint64_t GetUsedMemory();
    uint64_t GetReservedMemory();
    double GetPercentUsedMemory();
private:
    void InitBitmap(size_t bitmapSize, void *bufferAddress);
    void ReservePage(void *address);
    void UnreservePage(void *address);
    void ReservePages(void *address, uint64_t pageCount);
    void UnreservePages(void *address, uint64_t pageCount);
};

extern PageFrameAllocator GlobalAllocator;