#ifndef __AZERTY_KEYBOARD__H
#define __AZERTY_KEYBOARD__H

#include <stdint.h>

namespace KeyboardTranslation
{
    extern const char AZERTY[58];
    char azerty(uint8_t scancode, bool uppercase);
};

#endif