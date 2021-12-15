#pragma once

#include "../kernel.h"
#include <stdint.h>
#include "../memory/manager.h"

struct PeripheriqueInfos
{
    uint16_t device;
    uint16_t vendor;
    uint8_t classcode;

}__attribute__((packed));

class DriversLoader
{
public:
    DriversLoader();
    void save(uint16_t vendor, uint16_t device, uint8_t classcode);
private:
    void driverFlag();
    void noDriverFlag();
    uint8_t drivercount;
    PeripheriqueInfos *periph_infos;
    PeripheriqueInfos first_node_periph_info;
};