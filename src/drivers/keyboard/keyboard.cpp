#include "keyboard.h"

bool IsLeftShiftPressed;
bool IsRightShiftPressed;

/*
* To avoid repetions in the code my idea is to save all start addresses of the scancodes translations (QWERTY, AZERTY) and bc they
* have the same size, select the correct one with the same function
*/

Keyboard keyboard(QWERTY_TYPE); // This is the default configuration but it can be modify

Keyboard::Keyboard(uint8_t type)
{
    this->change_type(type);
    this->info();
}

Keyboard::~Keyboard(){}

void Keyboard::handle(uint8_t scancode)
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
    if(this->type == QWERTY_TYPE) ascii = KeyboardTranslation::qwerty(scancode, IsLeftShiftPressed | IsRightShiftPressed);
    else if(this->type == AZERTY_TYPE) ascii = KeyboardTranslation::azerty(scancode, IsLeftShiftPressed | IsRightShiftPressed);
    else basicRenderer.Errorln("Unknown keyboard type detected!");
    /*if(this->translation_adrs == NULL)
    {
        basicRenderer.Errorln("Unknown keyboard type detected!");
        return;
    }
    else
    {
        ascii = 
    }*/
    if(ascii != 0) basicRenderer.putChar(ascii);
    if(ascii == 'p') shutdown();
    return;
}

bool Keyboard::change_type(uint8_t type)
{
    switch(type)
    {
        case QWERTY_TYPE:
            this->type = QWERTY_TYPE;
            this->translation_adrs = &KeyboardTranslation::QWERTY[0];
            return true;
        case AZERTY_TYPE:
            this->type = AZERTY_TYPE;
            this->translation_adrs = &KeyboardTranslation::AZERTY[0];
            return true;
        default:
            this->translation_adrs = NULL;
            return false;
    }
}

void Keyboard::info()
{
    basicRenderer.Log("The keyboard is set to ");
    switch(this->type)
    {
        case QWERTY_TYPE:
            basicRenderer.Print("QWERTY");
            break;
        case AZERTY_TYPE:
            basicRenderer.Print("AZERTY");
            break;
        default:
            basicRenderer.Print("UNKNWON");
            break;
    }
    basicRenderer.Println("!");
    basicRenderer.Log("The start address of the char translations is 0x");
    basicRenderer.Print(ToHexString((uint64_t)this->translation_adrs));
    basicRenderer.Println("!");
    return;
}