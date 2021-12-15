#include "drivers.h"

DriversLoader::DriversLoader()
{
    basicRenderer.Logln("Starting drivers loader manager...");
    this->drivercount = 0;
    this->periph_infos = (PeripheriqueInfos*)memorymanager.malloc(sizeof(struct PeripheriqueInfos)*100);
    if(this->periph_infos == NULL)
    {
        panic("Error while allocating space in memory to store drivers data!");
        add_line("Size to allocate in bytes : ");
        add_line(ToString((uint64_t)sizeof(struct PeripheriqueInfos)*100));
        add_line_ln("!");
    }
    else
    {
        basicRenderer.Log("Address where the peripheriques infos will be saved : 0x");
        basicRenderer.Print(ToHexString((uint64_t)(uint64_t*)this->periph_infos));
        basicRenderer.Println("!");
    }
}

void DriversLoader::save(uint16_t vendor, uint16_t device, uint8_t classcode)
{
    this->drivercount++;
    this->periph_infos->vendor = vendor;
    this->periph_infos->device = device;
    this->periph_infos->classcode = classcode;
    this->periph_infos++;
    this->periph_infos->vendor = NULL;
    this->periph_infos->device = NULL;
    this->periph_infos->classcode = NULL;
    basicRenderer.OK();
}

void DriversLoader::driverFlag()
{
    basicRenderer.Println(" [DRIVER AVAILABLE]", GREEN);
}

void DriversLoader::noDriverFlag()
{
    basicRenderer.Println(" [DRIVER UNAVAILABLE]", RED);
}