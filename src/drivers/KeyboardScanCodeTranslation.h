#pragma once

#include <stdint.h>

#define LEFT_SHIFT 0x2A
#define RIGHT_SHIFT 0x36
#define ENTER 0x1C
#define BACK_SPACE 0x0E
#define SPACE_BAR 0x39
#define SHIFT_RELEASE 0x80

namespace QWERTYKeyboard
{
    extern const char ASCIITABLE[];
    char Translate(uint8_t scancode, bool uppercase);
};

namespace AZERTYKeyboard
{
    extern const char ASCIITABLE[];
    char Translate(uint8_t scancode, bool uppercase);
};