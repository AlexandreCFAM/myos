#include "mouse.h"
#include "../../IO.h"
#include <stdint.h>
#include "../../kernel.h"
#include "../../renderer.h"
#include "../../cstr.h"

uint8_t MousePointer[] = {
    0b00011000, 0b00011000,
    0b00011000, 0b00011000,
    0b00011000, 0b00011000,
    0b00011000, 0b00011000,
    0b00011000, 0b00011000,
    0b00011000, 0b00011000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
    0b01000000, 0b00000010,
    0b01100000, 0b00000110,
    0b01100000, 0b00000110,
    0b01100000, 0b00000110,
    0b01100000, 0b00000110,
    0b01100000, 0b00000110,
    0b01111111, 0b11111110,
    0b00111111, 0b11111100,

};

void InitPS2Mouse()
{
    outb(0x64, 0xA8);
    MouseWait();
    outb(0x64, 0x20);
    MouseWaitInput();
    uint8_t status = inb(0x60);
    status |= 0b10;
    MouseWait();
    outb(0x64, 0x60);
    MouseWait();
    outb(0x60, status);
    MouseWrite(0xF6);
    MouseRead();
    MouseWrite(0xF4);
    MouseRead();

}

void MouseWait()
{
    uint64_t timeout = 100000;
    while(timeout)
    {
        if(!(inb(0x64) & 0b10)) return;
        timeout--;
    }
}

void MouseWaitInput()
{
    uint64_t timeout = 100000;
    while(timeout)
    {
        if(inb(0x64) & 0b1) return;
        timeout--;
    }
}

void MouseWrite(uint8_t value)
{
    MouseWait();
    outb(0x64, 0xD4);
    MouseWait();
    outb(0x60, value);
}

uint8_t MouseRead()
{
    MouseWaitInput();
    return inb(0x60);
}

uint8_t MouseCycle = 0;
uint8_t MousePacket[4];
bool MousePackedReady = false;
long mouseX;
long mouseY;
long oldMouseX;
long oldMouseY;
void HandlePS2Mouse(uint8_t data)
{
    switch(MouseCycle)
    {
        case 0:
            if(MousePackedReady) break;
            if(!(data & 0b00001000)) break;
            MousePacket[0] = data;
            MouseCycle++;
            break;
        case 1:
            if(MousePackedReady) break;
            MousePacket[1] = data;
            MouseCycle++;
            break;
        case 2:
            if(MousePackedReady) break;
            MousePacket[2] = data;
            MousePackedReady = true;
            MouseCycle = 0;
            break;
    }
}

void ProcessMousePacket()
{
    if(!MousePackedReady) return;
    else MousePackedReady = false;
    bool Xnegative, Ynegative, xOverflow, yOverflow;
    if(MousePacket[0] & PS2XSIGN)
    {
        Xnegative = true;
    } else Xnegative = false;
    if(MousePacket[0] & PS2YSIGN)
    {
        Ynegative = true;
    } else Ynegative = false;
    if(MousePacket[0] & PS2XOVERFLOW) xOverflow = true; else xOverflow = false;
    if(MousePacket[0] & PS2YOVERFLOW) yOverflow = true; else yOverflow = false;
    if(!Xnegative)
    {
        mouseX += MousePacket[1];
        if(xOverflow) mouseX += 255;
    }
    else
    {
        MousePacket[1] = 256 - MousePacket[1];
         mouseX -= MousePacket[1];
        if(xOverflow) mouseX -= 255;
    }
    if(!Ynegative)
    {
        mouseY -= MousePacket[2];
        if(yOverflow) mouseY -= 255;
    }
    else
    {
        MousePacket[2] = 256 - MousePacket[2];
        mouseY += MousePacket[2];
        if(yOverflow) mouseY += 255;
    }
    if(mouseX < 0) mouseX = 0;
    if(mouseX > basicRenderer.framebuffer->Width - 1) mouseX = basicRenderer.framebuffer->Width - 8;
    if(mouseY < 0) mouseY = 0;
    if(mouseY > basicRenderer.framebuffer->Height - 16) mouseY = basicRenderer.framebuffer->Height - 16;
    basicRenderer.ClearMouseCursor(MousePointer, oldMouseX, oldMouseY);
    basicRenderer.DrawOverlayMouseCursor(MousePointer, mouseX, mouseY, WHITE);
    if(MousePacket[0] & PS2LEFTBUTTON)
    {
        basicRenderer.putIndependantChar('a', GREEN, (unsigned int)mouseX, (unsigned int)mouseY);
    }
    if(MousePacket[0] & PS2MIDDLEBUTTON)
    {
        basicRenderer.putIndependantChar('a', RED, (unsigned int)mouseX, (unsigned int)mouseY);
    }
    if(MousePacket[0] & PS2RIGHTBUTTON)
    {
        basicRenderer.putIndependantChar('a', BLUE, (unsigned int)mouseX, (unsigned int)mouseY);
    }
    MousePackedReady = false;
    oldMouseX = mouseX;
    oldMouseY = mouseY;
}