#include "acpi.h"
#include "../kernel.h"

/* XSDT is the root SDT */

struct SystemConfiguration MainSystemConfiguration;

ACPI::ACPI(struct RSDPDescriptorNewVersion *rsdp)
{
    basicRenderer.Log("RSDP Descriptor Pointer located at 0x");
    basicRenderer.Println(ToHexString((uint64_t)(uint64_t*)rsdp));
    this->_rsdp = rsdp;
    if(ACPI::IsNewVersionACPI()) basicRenderer.Successln("New ACPI version detected!"); else basicRenderer.Errorln("Bad ACPI version detected!");
    this->version = this->IsNewVersionACPI();
    if(ACPI::ValidateRSDPFirstPart())
    {
        basicRenderer.Successln("ACPI first sumup validated!");
        ACPI::validate = true;
    }
    else
    {
        basicRenderer.Errorln("ACPI first sumup failed!");
        ACPI::validate = false;
        panic("ACPI FIRST SUMUP FAILED!");
        add_line("VALUE OBTAINED => 0x");
        add_line(ToHexString(ACPI::GetValueValidateRSDPFirstPart() & 0x00000000000000FF));
        add_line(" instead of 0x0000000000000000");
        add_line_ln("!");
    }

    this->validate = true;

    if(this->version)
    {
        // If this is the new version, the kernel can use the XSDT instead of the RSDP
        this->xsdt_ptr = (XSDT*)this->_rsdp->XsdtAddress;
        basicRenderer.Log("XSDT located at 0x");
        basicRenderer.Println(ToHexString((uint64_t)this->xsdt_ptr));

        // Verify that the signature is correct
        if(!this->CheckSignature(&this->xsdt_ptr->header, (char*)"XSDT"))
        {
            panic("XSDT Signature not correct!");
            add_line("The XSDT signature must be \"XSDT\" but ");
            add_line(this->xsdt_ptr->header.Signature); // The signature printed isn't correct bc there is no 0 after the XSDT and it will print until it finds one of it
            add_line_ln(" has been detected!");
        } else basicRenderer.Successln("Correct XSDT signature found!");

        // Verify the table is valid
        if(!ACPI::IsChecksumCorrect(&this->xsdt_ptr->header))
        {
            panic("XSDT Checksum not correct!");
            add_line("The XSDT checksum is not equ to 0x0 mod 0x100 ! Value calculated => 0x");
            add_line(ToHexString((uint64_t)ACPI::Checksum(&this->xsdt_ptr->header)));
            add_line_ln("!");
        } else basicRenderer.Successln("XSDT cheksum validating ! Root System Descriptor defined!");
    }else; // Don't support for now the 1.0 version of the ACPI

    MainSystemConfiguration.xsdt = xsdt_ptr;

    // Now, we list all the tables available on the system

    this->ListTables();

    // Now we try to locate the FADT to begin
    struct FADT *fadt = (FADT*)this->FindTable((char*)"FACP");
    if(fadt == NULL)
    {
        panic("FADT is not found in the memory!");
    }else basicRenderer.Successln("FADT table selected!");

    // We detect many informations about the system thanks to this table

    MainSystemConfiguration.PowerManagementProfile = fadt->PreferredPowerManagementProfile;
    basicRenderer.Log("Power Management Profile detected as : ");
    switch(MainSystemConfiguration.PowerManagementProfile)
    {
        case 0:
            basicRenderer.Println("Undefined");
            break;
        case 1:
            basicRenderer.Println("Desktop");
            break;
        case 2:
            basicRenderer.Println("Mobile");
            break;
        case 3:
            basicRenderer.Println("WorkStation");
            break;
        case 4:
            basicRenderer.Println("Enterprise Server");
            break;
        case 5:
            basicRenderer.Println("SOHO Server");
            break;
        case 6:
            basicRenderer.Println("Aplliance PC");
            break;
        case 7:
            basicRenderer.Println("Performance Server");
            break;
        default:
            basicRenderer.Println("Reserved");
            break;
    }
    MainSystemConfiguration.dsdt = (uint64_t*)fadt->X_Dsdt;

    basicRenderer.Success("DSDT detected at 0x");
    basicRenderer.Print(ToHexString((uint64_t)MainSystemConfiguration.dsdt));
    basicRenderer.Println("!");
}

ACPI::~ACPI()
{
}

bool ACPI::IsNewVersionACPI()
{
    if(!this->_rsdp->first_part.Revision) return false; else return true;
}

uint64_t ACPI::GetValueValidateRSDPFirstPart()
{
    uint64_t sumup = 0;
    uint8_t number_of_bytes = sizeof(struct RSDPDescriptor);
    uint8_t *ptr = (uint8_t*)ACPI::_rsdp;
    for(uint8_t i = 0; i < number_of_bytes; i++)
    {
        sumup += (uint64_t)*ptr;
        ptr++;
    }
    return sumup;
}

bool ACPI::ValidateRSDPFirstPart()
{
    uint64_t sumup = this->GetValueValidateRSDPFirstPart();
    if(!(sumup & 0xFF)) return true; else return false;
}

uint64_t ACPI::GetValueValidateRSDPSecondPart()
{
    uint64_t sumup = 0;
    uint8_t number_of_bytes = sizeof(struct RSDPDescriptorNewVersion) - sizeof(struct RSDPDescriptor);
    uint8_t *ptr = (uint8_t*)(this->_rsdp->Length); // The address of the _rsdp is the beginning of the first part but I start after it, so at the address of Length
    for(uint8_t i = 0; i < number_of_bytes; i++)
    {
        sumup += (uint64_t)*ptr;
        ptr++;
    }
    return sumup;
}

bool ACPI::CheckSignature(SDTCommonHeader *table, char *signature)
{
    if(memcmp(&table->Signature[0], signature, 4)) return true; else return false;
}

unsigned char ACPI::Checksum(SDTCommonHeader *table)
{
    unsigned char sum = 0;
 
    for (int i = 0; i < table->Length; i++)
    {
        sum += ((char *) table)[i];
    }
 
    return sum;
}

bool ACPI::IsChecksumCorrect(SDTCommonHeader *table)
{
    return ACPI::Checksum(table) == 0;
}

SDTCommonHeader *ACPI::FindTable(char *signature)
{
    int entries = (this->xsdt_ptr->header.Length - sizeof(this->xsdt_ptr->header)) / 8;
    uint64_t *next_sdt = (uint64_t*)&this->xsdt_ptr->NextSDT;
    for(uint8_t i = 0; i < entries; i++)
    {
        SDTCommonHeader *header = (SDTCommonHeader*)*(uint64_t*)((uint64_t)this->xsdt_ptr + sizeof(SDTCommonHeader) + (i * 8));
        if(this->CheckSignature(header, signature)) return header; else;
    }
    return NULL;
}

void ACPI::ListTables()
{
    basicRenderer.Println("List of the tables found on the system : ");
    int entries = (this->xsdt_ptr->header.Length - sizeof(this->xsdt_ptr->header)) / 8;
    uint64_t *next_sdt = (uint64_t*)&this->xsdt_ptr->NextSDT;
    for(uint8_t i = 0; i < entries; i++)
    {
        SDTCommonHeader *header = (SDTCommonHeader*)*(uint64_t*)((uint64_t)this->xsdt_ptr + sizeof(SDTCommonHeader) + (i * 8));
        char signature[5];
        memcpy(&signature[0], &header->Signature[0], 4);
        signature[5] = '\0';
        basicRenderer.Print("   - ");
        basicRenderer.Print(signature);
        basicRenderer.Print(" at 0x");
        basicRenderer.Println(ToHexString((uint64_t)header));
    }
}