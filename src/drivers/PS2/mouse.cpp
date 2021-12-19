#include "mouse.h"

Mouse mouse;

Mouse::Mouse(){}

void Mouse::init()
{
    this->current_cursor = DefaultMousePointer;
    outb(0x64, 0xA8);
    this->wait();
    outb(0x64, 0x20);
    this->wait_input();
    uint8_t status = inb(0x60);
    status |= 0b10;
    this->wait();
    outb(0x64, 0x60);
    this->wait();
    outb(0x60, status);
    this->write(0xF6);
    this->read();
    this->write(0xF4);
    this->read();
    this->last_animation_done = PIT.Tick;
}

Mouse::~Mouse(){}

void Mouse::wait()
{
    uint64_t timeout = 100000;
    while(timeout)
    {
        if(!(inb(0x64) & 0b10)) return;
        timeout--;
    }
}

void Mouse::wait_input()
{
    uint64_t timeout = 100000;
    while(timeout)
    {
        if(inb(0x64) & 0b1) return;
        timeout--;
    }   
}

void Mouse::write(uint8_t value)
{
    this->wait();
    outb(0x64, 0xD4);
    this->wait();
    outb(0x60, value);
}

uint8_t Mouse::read()
{
    this->wait_input();
    return inb(0x60);
}

void Mouse::handle(uint8_t data)
{
    switch(this->MouseCycle)
    {
        case 0:
            if(this->MousePackedReady) break;
            if(!(data & 0b00001000)) break;
            this->MousePacket[0] = data;
            this->MouseCycle++;
            break;
        case 1:
            if(this->MousePackedReady) break;
            this->MousePacket[1] = data;
            this->MouseCycle++;
            break;
        case 2:
            if(this->MousePackedReady) break;
            this->MousePacket[2] = data;
            this->MousePackedReady = true;
            this->MouseCycle = 0;
            break;
    }
}

void Mouse::main()
{
    if(!this->MousePackedReady) return;
    else this->MousePackedReady = false;
    bool Xnegative, Ynegative, xOverflow, yOverflow;
    if(this->MousePacket[0] & PS2XSIGN)
    {
        Xnegative = true;
    } else Xnegative = false;
    if(this->MousePacket[0] & PS2YSIGN)
    {
        Ynegative = true;
    } else Ynegative = false;
    if(this->MousePacket[0] & PS2XOVERFLOW) xOverflow = true; else xOverflow = false;
    if(this->MousePacket[0] & PS2YOVERFLOW) yOverflow = true; else yOverflow = false;
    if(!Xnegative)
    {
        this->mouseX += this->MousePacket[1];
        if(xOverflow) this->mouseX += 255;
    }
    else
    {
        this->MousePacket[1] = 256 - this->MousePacket[1];
        this->mouseX -= this->MousePacket[1];
        if(xOverflow) this->mouseX -= 255;
    }
    if(!Ynegative)
    {
        this->mouseY -= this->MousePacket[2];
        if(yOverflow) this->mouseY -= 255;
    }
    else
    {
        this->MousePacket[2] = 256 - this->MousePacket[2];
        this->mouseY += this->MousePacket[2];
        if(yOverflow) this->mouseY += 255;
    }
    if(this->mouseX < 0) this->mouseX = 0;
    if(this->mouseX > basicRenderer.framebuffer->Width - 1) this->mouseX = basicRenderer.framebuffer->Width - 8;
    if(this->mouseY < 0) this->mouseY = 0;
    if(this->mouseY > basicRenderer.framebuffer->Height - 16) this->mouseY = basicRenderer.framebuffer->Height - 16;

    if(this->MousePacket[0] & PS2LEFTBUTTON)
    {
        this->hide();
        this->current_cursor = WhenLeftClickMousePointer;
        this->click.set(SET_LEFT_CLICK);
    }
    else if(this->MousePacket[0] & PS2MIDDLEBUTTON)
    {
        this->hide();
        this->current_cursor = WhenMiddleClickMousePointer;
        this->click.set(SET_MIDDLE_CLICK);
    }
    else if(this->MousePacket[0] & PS2RIGHTBUTTON)
    {
        this->hide();
        this->current_cursor = WhenRightClickMousePointer;
        this->click.set(SET_RIGHT_CLICK);
    }
    else
    {
        this->hide();
        this->click.set(CLEAR_ALL_CLICKS);
        this->current_cursor = DefaultMousePointer;
    }
    /*if(PIT.Tick - this->last_animation_done >= 7500)
    {
        this->bob_animation();
        this->last_animation_done = PIT.Tick;
    }
    else
    {
        this->hide();
        this->show(WHITE);
    }*/

    this->hide();
    this->show(WHITE);

    this->MousePackedReady = false;
    this->oldMouseX = this->mouseX;
    this->oldMouseY = this->mouseY;
}

void Mouse::show(uint32_t colour)
{
    this->hide();
    int futurx = this->mouseX;
    int futury = this->mouseY;
    int xMax = 16;
    int yMax = 16;
    int differenceX = basicRenderer.framebuffer->Width - this->mouseX;
    int differenceY = basicRenderer.framebuffer->Height - this->mouseY;
    if(differenceX < 16) xMax = differenceX;
    if(differenceY < 16) yMax = differenceY;
    for(int y = 0; y < yMax; y++)
    {
        for(int x = 0; x < xMax; x++)
        {
            int bit = y * HEIGHT_CHAR + x;
            int byte = bit / WIDTH_CHAR;
            if((this->current_cursor[byte] & (0b10000000) >> (x % 8)))
            {
                this->MouseCursorBuffer[x + y * 16] = basicRenderer.GetPixel(futurx + x, futury + y);
                basicRenderer.putPixel(futurx + x, futury + y, colour);
                this->MouseCursorBufferAfter[x + y * 16] = basicRenderer.GetPixel(futurx + x, futury + y);
            }
        }
    }
    this->MouseDrawn = true;
}

void Mouse::hide()
{
    if(!this->MouseDrawn) return;
    int futurx = this->oldMouseX;
    int futury = this->oldMouseY;
    int xMax = 16;
    int yMax = 16;
    int differenceX = basicRenderer.framebuffer->Width - this->oldMouseX;
    int differenceY = basicRenderer.framebuffer->Height - this->oldMouseY;
    if(differenceX < 16) xMax = differenceX;
    if(differenceY < 16) yMax = differenceY;
    for(int y = 0; y < yMax; y++)
    {
        for(int x = 0; x < xMax; x++)
        {
            int bit = y * HEIGHT_CHAR + x;
            int byte = bit / WIDTH_CHAR;
            if((this->current_cursor[byte] & (0b10000000) >> (x % 8)))
            {
                if(basicRenderer.GetPixel(futurx + x, futury + y) == this->MouseCursorBufferAfter[x + y * 16])
                {
                    basicRenderer.putPixel(futurx + x, futury + y, this->MouseCursorBuffer[x + y * 16]);
                }
            }
        }
    }
}

void Mouse::bob_animation()
{
    uint8_t *last_cursor_selected = this->current_cursor;
    for(char i = 0; i < 1; i++)
    {
        this->hide();
        this->current_cursor = WhenMiddleClickMousePointer;
        this->show(WHITE);
        PIT.sleep_milliseconds(200);
        this->hide();
        this->current_cursor = last_cursor_selected;
        this->show(WHITE);
        PIT.sleep_milliseconds(100);
    }
}

void Click::set(uint8_t value)
{
    switch(value)
    {
        case CLEAR_ALL_CLICKS:
            this->left = this->middle = this->right = 0;
            break;
        case SET_LEFT_CLICK:
            this->left = 1;
            this->middle = this->right = 0;
            break;
        case SET_MIDDLE_CLICK:
            this->middle = 1;
            this->left = this->right = 0;
            break;
        case SET_RIGHT_CLICK:
            this->right = 1;
            this->left = this->middle = 0;
            break;
        case SET_ALL_CLICKS:
            this->left = this->middle = this->right = 0;
            break;
        default: break;
    }
}