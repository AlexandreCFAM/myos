#include "IDT.h"

void IDTDescriptorEntry::SetOffset(uint64_t offset)
{
    offset0 = (uint16_t)(offset & 0x000000000000FFFF);
    offset1 = (uint16_t)((offset & 0x00000000FFFF0000) >> 16);
    offset2 = (uint32_t)((offset & 0xFFFFFFFF00000000) >> 32);
}

uint64_t IDTDescriptorEntry::GetOffset()
{
    uint64_t offset = 0;
    offset |= (uint64_t)offset0;
    offset |= (uint64_t)offset1 << 16;
    offset |= (uint64_t)offset2 << 32;
    return offset;
}

void SetIDTGate(void *handler, uint8_t entryOffset, uint8_t type_attr, uint8_t selector)
{
    IDTDescriptorEntry *interrupt =(IDTDescriptorEntry*)(idtr.Offset + entryOffset * sizeof(IDTDescriptorEntry));
    interrupt->SetOffset((uint64_t)handler);
    interrupt->type_attr = type_attr;
    interrupt->selector = selector;

}