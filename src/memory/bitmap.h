#pragma once

#include <stdint.h>
#include "../lib/defs.h"

// #define BITMAP_SIZE 20

class Bitmap
{
public:
    size_t Size;
    uint8_t *Buffer;
    bool operator[](uint64_t index);
    bool Set(uint64_t index, bool value);
};