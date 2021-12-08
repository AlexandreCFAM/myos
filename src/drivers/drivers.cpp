#include "drivers.h"

DriversLoader::DriversLoader()
{
    basicRenderer.Logln("Starting drivers loader manager...");
    DriversLoader::drivercount = 0;
}

bool DriversLoader::Load(uint16_t vendor, uint16_t device, uint8_t classcode)
{
    DriversLoader::noDriverFlag();
    return false;
}

void DriversLoader::driverFlag()
{
    basicRenderer.Println(" [DRIVER AVAILABLE]", GREEN);
}

void DriversLoader::noDriverFlag()
{
    basicRenderer.Println(" [DRIVER UNAVAILABLE]", RED);
}

uint64_t *DriversLoader::ReservePlace()
{
    if(DriversLoader::drivercount >= 255) return NULL;
    else
    {
        DriversLoader::drivercount++;
        return (uint64_t*)&DriversLoader::drivers[DriversLoader::drivercount];
    }
}