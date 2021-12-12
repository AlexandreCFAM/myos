#include "fadt.h"

FixedACPIDescriptionTable::FixedACPIDescriptionTable(FADT *fadt)
{
    this->fadt = fadt;
}

FixedACPIDescriptionTable::~FixedACPIDescriptionTable(){}

void FixedACPIDescriptionTable::main()
{
    this->powermanagement();
}

void FixedACPIDescriptionTable::powermanagement()
{
    MainSystemConfiguration.PowerManagementProfile = this->fadt->PreferredPowerManagementProfile;
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
}