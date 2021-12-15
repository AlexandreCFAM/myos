#pragma once

#include <stdint.h>

#define LEFT_SHIFT 0x2A
#define RIGHT_SHIFT 0x36
#define ENTER 0x1C
#define BACK_SPACE 0x0E
#define SPACE_BAR 0x39
#define SHIFT_RELEASE 0x80

namespace KeyboardTranslation
{
    extern const char QWERTY[58];
    char qwerty(uint8_t scancode, bool uppercase);
};