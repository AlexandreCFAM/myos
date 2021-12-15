#pragma once

#include <stdint.h>
#include "translations/qwerty.h"
#include "translations/azerty.h"
#include "../../kernel.h"
#include "../../lib/cstr.h"
#include "../../hardware/shutdown.h"

#define QWERTY_TYPE 0
#define AZERTY_TYPE 1

class Keyboard
{
public:
    Keyboard(uint8_t type);
    ~Keyboard();
    void handle(uint8_t scancode);
    bool change_type(uint8_t type);
    void info();
private:
    uint8_t type;
    const char *translation_adrs;
};

extern Keyboard keyboard;