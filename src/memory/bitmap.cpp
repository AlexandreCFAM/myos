#include "bitmap.h"

bool Bitmap::operator[](uint64_t index)
{
    if(index > Size *8) return false;
    //Get byte index of the bit
    uint64_t ByteIndex = index / 8;
    //Get number of bit in byte
    uint8_t BitIndex = index % 8;
    uint8_t BitIndexer = 0b10000000 >> BitIndex;
    if(Buffer[ByteIndex] & BitIndexer) return true; else return false;

}

bool Bitmap::Set(uint64_t index, bool value)
{
    if(index > Size * 8) return false;
    //Get byte index of the bit
    uint64_t ByteIndex = index / 8;
    //Get number of bit in byte
    uint8_t BitIndex = index % 8;
    uint8_t BitIndexer = 0b10000000 >> BitIndex;
    Buffer[ByteIndex] &= ~BitIndexer;
    if(value) Buffer[ByteIndex] |= BitIndexer;
    return true;
}