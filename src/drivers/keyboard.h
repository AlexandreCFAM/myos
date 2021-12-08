#pragma once

#include <stdint.h>
#include "KeyboardScanCodeTranslation.h"
#include "../kernel.h"
#include "../cstr.h"
#include "../shutdown.h"

#define QWERTY 0
#define AZERTY 1

void HandleKeyboard(uint8_t scancode);
void ChoiceKeyboard(bool keyboard);
extern bool keyboardUsed;