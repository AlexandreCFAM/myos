#pragma once

#include <stdint.h>

#define PS2LEFTBUTTON 0b00000001
#define PS2MIDDLEBUTTON 0b00000100
#define PS2RIGHTBUTTON 0b00000010

#define PS2XSIGN 0b00010000
#define PS2YSIGN 0b00100000
#define PS2XOVERFLOW 0b01000000
#define PS2YOVERFLOW 0b10000000

void InitPS2Mouse();
void MouseWait();
void MouseWaitInput();
void MouseWrite(uint8_t value);
uint8_t MouseRead();
void HandlePS2Mouse(uint8_t data);
void ProcessMousePacket();

extern long mouseX;
extern long mouseY;
extern uint8_t MousePointer[];