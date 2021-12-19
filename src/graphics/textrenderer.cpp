#include "textrenderer.h"
#include "../kernel.h"
#include "../memory.h"
#include "../lib/cstr.h"
#include <stdint.h>

TextRenderer::TextRenderer(FrameBuffer *_framebuffer, PSF1_FONT *_psf1_font)
{
    framebuffer = _framebuffer;
    psf1_font = _psf1_font;
    MainTextCursor.x = 0;
    MainTextCursor.y = 0;
    ClearColour = 0;
    offset = 0;
}

TextRenderer::~TextRenderer(){}

void TextRenderer::putChar(char c, unsigned int colour, unsigned int _x, unsigned int _y)
{
    TextRenderer::CurrentColour = colour;
    unsigned int *pixptr = (unsigned int*)TextRenderer::framebuffer->BaseAddress;
    char *fontptr = (char*)TextRenderer::psf1_font->glyphBuffer + (c * TextRenderer::psf1_font->psf1_header->CharSize);
    for(unsigned long y = _y; y < _y + HEIGHT_CHAR; y++)
    {
        for(unsigned long x = _x; x < _x + WIDTH_CHAR; x++)
        {
            if((*fontptr & (0b10000000 >> (x - _x))) > 0) *(unsigned int*)(pixptr + x + (y * framebuffer->PixelsPerScanLine)) = colour;
        }
        fontptr++;
    }
    MainTextCursor.x += WIDTH_CHAR;
    if(MainTextCursor.x >= TextRenderer::framebuffer->Width)
    {
        MainTextCursor.x = 0;
        MainTextCursor.y += HEIGHT_CHAR;
    }
    if(MainTextCursor.y >= 700)
    {
        this->scroll(1);
    }
}

void TextRenderer::putChar(char c)
{
    this->putChar(c, WHITE, MainTextCursor.x, MainTextCursor.y);
}

void TextRenderer::putIndependantChar(char c, unsigned int colour, unsigned int _x, unsigned int _y)
{
    unsigned int lastX = MainTextCursor.x;
    unsigned int lastY = MainTextCursor.y;
    this->putChar(c, colour, _x, _y);
    MainTextCursor.x = lastX;
    MainTextCursor.y = lastY;
}

void TextRenderer::ClearChar()
{
    unsigned int xOff = MainTextCursor.x;
    unsigned int yOff = MainTextCursor.y;

    MainTextCursor.x -= WIDTH_CHAR;

    if(MainTextCursor.x < 0)
    {
        MainTextCursor.x = bootInfo->framebuffer->Width;
        MainTextCursor.y -= HEIGHT_CHAR;
    }

    unsigned int* pixPtr = (unsigned int*)this->framebuffer->BaseAddress;
    for (unsigned long y = yOff; y < yOff + HEIGHT_CHAR; y++){
        for (unsigned long x = xOff - WIDTH_CHAR; x < xOff; x++){
                    *(unsigned int*)(pixPtr + x + (y * this->framebuffer->PixelsPerScanLine)) = ClearColour;
        }
    }
}

void TextRenderer::Print(const char *text, unsigned int colour)
{
    char *c = (char*)text;
    while(*c != 0)
    {
        TextRenderer::putChar(*c, colour, MainTextCursor.x + offset, MainTextCursor.y);
        c++;
    }
}

void TextRenderer::Println(const char *text, unsigned int colour)
{
    TextRenderer::Print(text, colour);
    MainTextCursor.y += HEIGHT_CHAR;
    MainTextCursor.x = 0;
    if(MainTextCursor.y > TextRenderer::framebuffer->Height) MainTextCursor.y = 0;
}

void TextRenderer::clear()
{
    TextRenderer::clear(BLACK);
}

void TextRenderer::clear(unsigned int colour)
{
    uint64_t fbBase = (uint64_t)TextRenderer::framebuffer->BaseAddress;
    uint64_t BytesPerScanLine = TextRenderer::framebuffer->PixelsPerScanLine * 4;
    uint64_t fbHeight = TextRenderer::framebuffer->Height;
    uint64_t fbSize = TextRenderer::framebuffer->BufferSize;
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

void TextRenderer::Print(const char *text)
{
    TextRenderer::Print(text, WHITE);
}

void TextRenderer::Println(const char *text)
{
    TextRenderer::Println(text, WHITE);
}

void TextRenderer::OK()
{
    TextRenderer::Print(" [");
    TextRenderer::Print("OK", GREEN);
    TextRenderer::Println("]");
}

void TextRenderer::Log(const char *text)
{
    TextRenderer::Print("[LOG] ");
    TextRenderer::Print(text);
}

void TextRenderer::Logln(const char *text)
{
    TextRenderer::Print("[LOG] ");
    TextRenderer::Println(text);
}


void TextRenderer::putPixel(uint32_t x, uint32_t y, uint32_t color)
{
    *(uint32_t*)((uint64_t)TextRenderer::framebuffer->BaseAddress + (x * 4) + (y * TextRenderer::framebuffer->PixelsPerScanLine * 4)) = color;
}

uint32_t TextRenderer::GetPixel(uint32_t x, uint32_t y)
{
    return *(uint32_t*)((uint64_t)TextRenderer::framebuffer->BaseAddress + (x * 4) + (y * TextRenderer::framebuffer->PixelsPerScanLine * 4));
}

void TextRenderer::scroll(uint8_t n)
{
    // Need to clear mouse cursor before scrolling
    mouse.hide();
    uint32_t *base = (uint32_t*)TextRenderer::framebuffer->BaseAddress;
    uint32_t pixnumber = n * TextRenderer::framebuffer->PixelsPerScanLine * HEIGHT_CHAR;
    uint32_t *end = (uint32_t*)(base + (uint32_t)(TextRenderer::framebuffer->Width * TextRenderer::framebuffer->Height));
    uint32_t *start = (uint32_t*)(base + pixnumber);
    for(uint32_t *pixel = start; pixel < end; pixel++) *(pixel - pixnumber) = *pixel;
    MainTextCursor.y -= n * HEIGHT_CHAR;
}

void TextRenderer::verif_coos_for_scroll()
{
    if(MainTextCursor.y >= TextRenderer::framebuffer->Height) TextRenderer::scroll(1); else;
}

void TextRenderer::Error(const char *text)
{
    TextRenderer::Print("[");
    TextRenderer::Print("ERROR", RED);
    TextRenderer::Print("] ");
    TextRenderer::Print(text);
}

void TextRenderer::Errorln(const char *text)
{
    TextRenderer::Print("[");
    TextRenderer::Print("ERROR", RED);
    TextRenderer::Print("] ");
    TextRenderer::Println(text);
}

void TextRenderer::ErrorFlag()
{
    TextRenderer::Println(" [ERROR]", RED);
}

void TextRenderer::Success(const char *text)
{
    TextRenderer::Print("[");
    TextRenderer::Print("SUCCESS", GREEN);
    TextRenderer::Print("] ");
    TextRenderer::Print(text);
}

void TextRenderer::Successln(const char *text)
{
    TextRenderer::Print("[");
    TextRenderer::Print("SUCCESS", GREEN);
    TextRenderer::Print("] ");
    TextRenderer::Println(text);
}

void TextRenderer::SuccessFlag()
{
    TextRenderer::Println(" [SUCCESS]", GREEN);
}