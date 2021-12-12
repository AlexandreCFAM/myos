#ifndef __PIT__H
#define __PIT__H

#define PIT_FREQUENCY 1193182

#include "../IO.h"
#include <stdint.h>

struct ProgrammableIntervalTimer
{
    uint16_t frequency;
};

extern struct ProgrammableIntervalTimer PIT;

#endif