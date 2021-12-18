#include "apic.h"

APIC apic_class;

APIC::APIC(){}

void APIC::init(SDTCommonHeader *apic)
{
    this->whereami = (APIC_HEADER*)apic;
    basicRenderer.Log("Length of the APIC table : ");
    basicRenderer.Print(ToString((uint64_t)this->whereami->header.Length));
    basicRenderer.Println(" bytes!");
    basicRenderer.Log("Local APIC address : 0x");
    basicRenderer.Print(ToHexString((uint64_t)this->whereami->apic_local_address));
    basicRenderer.Println("!");
    
    // Verify if bit 0 is set in flags to determine if we need to mask all PIC interrupts
    if(this->whereami->flags && 0b1)
    {
        // Mask all the interrupts of the PIC
        // MaskAllInterrupts();
    }
}

APIC::~APIC(){}
