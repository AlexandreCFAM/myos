#include "keyboard.h"

bool IsLeftShiftPressed;
bool IsRightShiftPressed;
bool keyboardUsed = QWERTY;

void HandleKeyboard(uint8_t scancode)
{
    switch(scancode)
    {
        case LEFT_SHIFT:
            IsLeftShiftPressed = true;
            return;
        case LEFT_SHIFT + SHIFT_RELEASE:
            IsLeftShiftPressed = false;
            return;
        case RIGHT_SHIFT:
            IsRightShiftPressed = true;
            return;
        case RIGHT_SHIFT + SHIFT_RELEASE:
            IsRightShiftPressed = false;
            return;
        case ENTER:
            basicRenderer.Println("");
            break;
        case SPACE_BAR:
            basicRenderer.putChar(' ');
            return;
        case BACK_SPACE:
            basicRenderer.ClearChar();
            return;
    }
    char ascii;
    if(keyboardUsed == QWERTY) ascii = QWERTYKeyboard::Translate(scancode, IsLeftShiftPressed | IsRightShiftPressed);
    else ascii = AZERTYKeyboard::Translate(scancode, IsLeftShiftPressed | IsRightShiftPressed);
    if(ascii != 0) basicRenderer.putChar(ascii);
    if(ascii == 'p') shutdown();
    
}

void ChoiceKeyboard(bool keyboard)
{
    keyboardUsed = keyboard;
}