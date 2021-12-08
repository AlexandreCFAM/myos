#pragma once

#include "../kernel.h"

class DriversLoader
{
public:
    DriversLoader();
    bool Load(uint16_t vendor, uint16_t device, uint8_t classcode);
private:
    void driverFlag();
    void noDriverFlag();
    uint64_t drivers[256];
    uint8_t drivercount;
    uint64_t *ReservePlace();
};