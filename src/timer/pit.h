#ifndef __PIT__H
#define __PIT__H

#define PIT_FREQUENCY 1193182

#include "../hardware/IO.h"
#include "../kernel.h"
#include <stdint.h>

struct ProgrammableIntervalTimer
{
    uint16_t frequency;
    uint64_t Tick;
    void init(uint16_t divisor);
    void sleep(uint8_t n);
    void sleep_milliseconds(uint16_t n);
};

extern struct ProgrammableIntervalTimer PIT;

#endif
