#include "azerty.h"

const char KeyboardTranslation::AZERTY[58] = {
         0 ,  0 , '1', '2',
        '3', '4', '5', '6',
        '7', '8', '9', '0',
        '-', '=',  0 ,  0 ,
        'a', 'z', 'e', 'r',
        't', 'y', 'u', 'i',
        'o', 'p', '[', ']',
         0 ,  0 , 'q', 's',
        'd', 'f', 'g', 'h',
        'j', 'k', 'l', 'm',
        '\'','`',  0 , '\\',
        'w', 'x', 'c', 'v',
        'b', 'n', ',', ';',
        ':', '!',  0 , '*',
         0 , ' '
};

char KeyboardTranslation::azerty(uint8_t scancode, bool uppercase)
{
    if(scancode > 58) return 0;
    if(uppercase) return KeyboardTranslation::AZERTY[scancode] - 32;
    else return KeyboardTranslation::AZERTY[scancode];
}