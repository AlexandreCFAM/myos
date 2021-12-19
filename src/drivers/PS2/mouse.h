#pragma once

#include <stdint.h>
#include "../../hardware/IO.h"
#include "../../kernel.h"
#include "../../graphics/textrenderer.h"
#include "../../lib/cstr.h"
#include "cursors.h"

#define PS2LEFTBUTTON 0b00000001
#define PS2MIDDLEBUTTON 0b00000100
#define PS2RIGHTBUTTON 0b00000010

#define PS2XSIGN 0b00010000
#define PS2YSIGN 0b00100000
#define PS2XOVERFLOW 0b01000000
#define PS2YOVERFLOW 0b10000000

#define CLEAR_ALL_CLICKS 0
#define SET_LEFT_CLICK 1
#define SET_MIDDLE_CLICK 2
#define SET_RIGHT_CLICK 3
#define SET_ALL_CLICKS 4

void InitPS2Mouse();
void MouseWait();
void MouseWaitInput();
void MouseWrite(uint8_t value);
uint8_t MouseRead();
void HandlePS2Mouse(uint8_t data);
void ProcessMousePacket();

// Just code style this struct

struct Click
{
    bool left;
    bool middle;
    bool right;
    void set(uint8_t value);
}__attribute__((packed));

class Mouse
{
public:
    Mouse();
    ~Mouse();
    void init();
    void handle(uint8_t data);
    void main();
    void show(uint32_t color);
    void hide();
    void bob_animation();
    long mouseX;
    long mouseY;
    long oldMouseX;
    long oldMouseY;
    struct Click click;
private:
    void wait();
    void wait_input();
    void write(uint8_t value);
    uint8_t read();
    uint32_t MouseCursorBuffer[16 * 16];
    uint32_t MouseCursorBufferAfter[16 * 16];
    bool MouseDrawn;
    uint8_t MouseCycle = 0;
    uint8_t MousePacket[4];
    bool MousePackedReady = false;
    uint8_t *current_cursor;
    uint64_t last_animation_done;
};

extern Mouse mouse;