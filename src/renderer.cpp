#include "renderer.h"
#include "kernel.h"
#include "memory.h"
#include "lib/cstr.h"
#include <stdint.h>

#define WIDTH_CHAR 8
#define HEIGHT_CHAR 16

BasicRenderer::BasicRenderer(FrameBuffer *_framebuffer, PSF1_FONT *_psf1_font)
{
    framebuffer = _framebuffer;
    psf1_font = _psf1_font;
    MainTextCursor.x = 0;
    MainTextCursor.y = 0;
    ClearColour = 0;
    offset = 0;
}

BasicRenderer::~BasicRenderer(){}

void BasicRenderer::putChar(char c, unsigned int colour, unsigned int _x, unsigned int _y)
{
    BasicRenderer::CurrentColour = colour;
    unsigned int *pixptr = (unsigned int*)BasicRenderer::framebuffer->BaseAddress;
    char *fontptr = (char*)BasicRenderer::psf1_font->glyphBuffer + (c * BasicRenderer::psf1_font->psf1_header->CharSize);
    for(unsigned long y = _y; y < _y + HEIGHT_CHAR; y++)
    {
        for(unsigned long x = _x; x < _x + WIDTH_CHAR; x++)
        {
            if((*fontptr & (0b10000000 >> (x - _x))) > 0) *(unsigned int*)(pixptr + x + (y * framebuffer->PixelsPerScanLine)) = colour;
        }
        fontptr++;
    }
    MainTextCursor.x += WIDTH_CHAR;
    if(MainTextCursor.x >= BasicRenderer::framebuffer->Width)
    {
        MainTextCursor.x = 0;
        MainTextCursor.y += HEIGHT_CHAR;
    }
    if(MainTextCursor.y >= 976)
    {
        BasicRenderer::scroll(1);
    }
}

void BasicRenderer::putChar(char c)
{
    BasicRenderer::putChar(c, WHITE, MainTextCursor.x, MainTextCursor.y);
}

void BasicRenderer::putIndependantChar(char c, unsigned int colour, unsigned int _x, unsigned int _y)
{
    unsigned int lastX = MainTextCursor.x;
    unsigned int lastY = MainTextCursor.y;
    BasicRenderer::putChar(c, colour, _x, _y);
    MainTextCursor.x = lastX;
    MainTextCursor.y = lastY;
}

void BasicRenderer::ClearChar()
{
    unsigned int xOff = MainTextCursor.x;
    unsigned int yOff = MainTextCursor.y;

    MainTextCursor.x -= WIDTH_CHAR;

    if(MainTextCursor.x < 0)
    {
        MainTextCursor.x = bootInfo->framebuffer->Width;
        MainTextCursor.y -= HEIGHT_CHAR;
    }

    unsigned int* pixPtr = (unsigned int*)BasicRenderer::framebuffer->BaseAddress;
    for (unsigned long y = yOff; y < yOff + HEIGHT_CHAR; y++){
        for (unsigned long x = xOff - WIDTH_CHAR; x < xOff; x++){
                    *(unsigned int*)(pixPtr + x + (y * BasicRenderer::framebuffer->PixelsPerScanLine)) = ClearColour;
        }
    }
}

void BasicRenderer::Print(const char *text, unsigned int colour)
{
    char *c = (char*)text;
    while(*c != 0)
    {
        BasicRenderer::putChar(*c, colour, MainTextCursor.x + offset, MainTextCursor.y);
        c++;
    }
}

void BasicRenderer::Println(const char *text, unsigned int colour)
{
    BasicRenderer::Print(text, colour);
    MainTextCursor.y += HEIGHT_CHAR;
    MainTextCursor.x = 0;
    if(MainTextCursor.y > BasicRenderer::framebuffer->Height) MainTextCursor.y = 0;
}

void BasicRenderer::clear()
{
    BasicRenderer::clear(BLACK);
}

void BasicRenderer::clear(unsigned int colour)
{
    uint64_t fbBase = (uint64_t)BasicRenderer::framebuffer->BaseAddress;
    uint64_t BytesPerScanLine = BasicRenderer::framebuffer->PixelsPerScanLine * 4;
    uint64_t fbHeight = BasicRenderer::framebuffer->Height;
    uint64_t fbSize = BasicRenderer::framebuffer->BufferSize;
    for(int verticalScanLine = 0; verticalScanLine < fbHeight; verticalScanLine++)
    {
        uint64_t pixelptrbase = fbBase + (BytesPerScanLine * verticalScanLine);
        for(uint32_t *pixptr = (uint32_t*)pixelptrbase; pixptr < (uint32_t*)(pixelptrbase + BytesPerScanLine); pixptr++)
        {
            *pixptr = colour;
        }
    }
    MainTextCursor.x = 0;
    MainTextCursor.y = 0;
}

void BasicRenderer::Print(const char *text)
{
    BasicRenderer::Print(text, WHITE);
}

void BasicRenderer::Println(const char *text)
{
    BasicRenderer::Println(text, WHITE);
}

void BasicRenderer::OK()
{
    BasicRenderer::Print(" [");
    BasicRenderer::Print("OK", GREEN);
    BasicRenderer::Println("]");
}

void BasicRenderer::Log(const char *text)
{
    BasicRenderer::Print("[LOG] ");
    BasicRenderer::Print(text);
}

void BasicRenderer::Logln(const char *text)
{
    BasicRenderer::Print("[LOG] ");
    BasicRenderer::Println(text);
}

void BasicRenderer::DrawOverlayMouseCursor(uint8_t *MouseCursor, long x, long y, uint32_t colour)
{
    int futurx = x;
    int futury = y;
    int xMax = 16;
    int yMax = 16;
    int differenceX = BasicRenderer::framebuffer->Width - x;
    int differenceY = BasicRenderer::framebuffer->Height - y;
    if(differenceX < 16) xMax = differenceX;
    if(differenceY < 16) yMax = differenceY;
    for(int y = 0; y < yMax; y++)
    {
        for(int x = 0; x < xMax; x++)
        {
            int bit = y * HEIGHT_CHAR + x;
            int byte = bit / WIDTH_CHAR;
            if((MouseCursor[byte] & (0b10000000) >> (x % 8)))
            {
                MouseCursorBuffer[x + y * 16] = GetPixel(futurx + x, futury + y);
                putPixel(futurx + x, futury + y, colour);
                MouseCursorBufferAfter[x + y * 16] = GetPixel(futurx + x, futury + y);
            }
        }
    }
    BasicRenderer::MouseDrawn = true;
}

void BasicRenderer::putPixel(uint32_t x, uint32_t y, uint32_t color)
{
    *(uint32_t*)((uint64_t)BasicRenderer::framebuffer->BaseAddress + (x * 4) + (y * BasicRenderer::framebuffer->PixelsPerScanLine * 4)) = color;
}

uint32_t BasicRenderer::GetPixel(uint32_t x, uint32_t y)
{
    return *(uint32_t*)((uint64_t)BasicRenderer::framebuffer->BaseAddress + (x * 4) + (y * BasicRenderer::framebuffer->PixelsPerScanLine * 4));
}

void BasicRenderer::ClearMouseCursor(uint8_t *mouseCursor, long x, long y)
{
    if(!BasicRenderer::MouseDrawn) return;
    int futurx = x;
    int futury = y;
    int xMax = 16;
    int yMax = 16;
    int differenceX = BasicRenderer::framebuffer->Width - x;
    int differenceY = BasicRenderer::framebuffer->Height - y;
    if(differenceX < 16) xMax = differenceX;
    if(differenceY < 16) yMax = differenceY;
    for(int y = 0; y < yMax; y++)
    {
        for(int x = 0; x < xMax; x++)
        {
            int bit = y * HEIGHT_CHAR + x;
            int byte = bit / WIDTH_CHAR;
            if((mouseCursor[byte] & (0b10000000) >> (x % 8)))
            {
                if(GetPixel(futurx + x, futury + y) == MouseCursorBufferAfter[x + y * 16])
                {
                    putPixel(futurx + x, futury + y, MouseCursorBuffer[x + y * 16]);
                }
            }
        }
    }
}

void BasicRenderer::scroll(uint8_t n)
{
    uint32_t *base = (uint32_t*)BasicRenderer::framebuffer->BaseAddress;
    uint32_t pixnumber = n * BasicRenderer::framebuffer->PixelsPerScanLine * HEIGHT_CHAR;
    uint32_t *end = (uint32_t*)(base + (uint32_t)(BasicRenderer::framebuffer->Width * BasicRenderer::framebuffer->Height));
    uint32_t *start = (uint32_t*)(base + pixnumber);
    for(uint32_t *pixel = start; pixel < end; pixel++) *(pixel - pixnumber) = *pixel;
    MainTextCursor.y -= n * HEIGHT_CHAR;
}

void BasicRenderer::verif_coos_for_scroll()
{
    if(MainTextCursor.y >= BasicRenderer::framebuffer->Height) BasicRenderer::scroll(1); else;
}

void BasicRenderer::Error(const char *text)
{
    BasicRenderer::Print("[");
    BasicRenderer::Print("ERROR", RED);
    BasicRenderer::Print("] ");
    BasicRenderer::Print(text);
}

void BasicRenderer::Errorln(const char *text)
{
    BasicRenderer::Print("[");
    BasicRenderer::Print("ERROR", RED);
    BasicRenderer::Print("] ");
    BasicRenderer::Println(text);
}

void BasicRenderer::ErrorFlag()
{
    BasicRenderer::Println(" [ERROR]", RED);
}

void BasicRenderer::Success(const char *text)
{
    BasicRenderer::Print("[");
    BasicRenderer::Print("SUCCESS", GREEN);
    BasicRenderer::Print("] ");
    BasicRenderer::Print(text);
}

void BasicRenderer::Successln(const char *text)
{
    BasicRenderer::Print("[");
    BasicRenderer::Print("SUCCESS", GREEN);
    BasicRenderer::Print("] ");
    BasicRenderer::Println(text);
}

void BasicRenderer::SuccessFlag()
{
    BasicRenderer::Println(" [SUCCESS]", GREEN);
}