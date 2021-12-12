#ifndef __RENDERER__H
#define __RENDERER__H

#define WHITE 0xFFFFFFFF
#define RED 0x00FF0000
#define BLACK 0x00000000
#define BLUE 0x000000FF
#define GREEN 0x0000FF00
#define YELLOW 0x00FFFF00

#include "defs.h"
#include "lib/math.h"
#include <stdint.h>

struct FrameBuffer
{
    void *BaseAddress;
    size_t BufferSize;
    unsigned int Width;
    unsigned int Height;
    unsigned int PixelsPerScanLine;
};

struct PSF1_HEADER
{
    unsigned char MagicBytes[2];
    unsigned char mode;
    unsigned char CharSize;
};

struct PSF1_FONT
{
    PSF1_HEADER *psf1_header;
    void *glyphBuffer;
};

/*struct TextCursor
{
    unsigned int x;
    unsigned int y;
    void verifCoos(FrameBuffer framebuffer);
};*/

class BasicRenderer
{
public:
    BasicRenderer(FrameBuffer *_framebuffer, PSF1_FONT *_psf1_font);
    ~BasicRenderer();
    void putChar(char c, unsigned int colour, unsigned int _x, unsigned int _y);
    void putChar(char c);
    void putIndependantChar(char c, unsigned int colour, unsigned int _x, unsigned int _y);
    void Print(const char *text, unsigned int colour);
    void Println(const char *text, unsigned int colour);
    void clear();
    void clear(unsigned int colour);
    void ClearChar();
    void Print(const char *text);
    void Println(const char *text);
    void OK();
    void Log(const char *text);
    void Logln(const char *text);
    void DrawOverlayMouseCursor(uint8_t *MouseCursor, long x, long y, uint32_t colour);
    void putPixel(uint32_t x, uint32_t y, uint32_t color);
    uint32_t GetPixel(uint32_t x, uint32_t y);
    void ClearMouseCursor(uint8_t *mouseCursor, long x, long y);
    void scroll(uint8_t n);
    void verif_coos_for_scroll();
    void Error(const char *text);
    void Errorln(const char *text);
    void ErrorFlag();
    void Success(const char *text);
    void Successln(const char *text);
    void SuccessFlag();

    FrameBuffer *framebuffer;
    PSF1_FONT *psf1_font;
    unsigned int ClearColour;
    uint32_t MouseCursorBuffer[16 * 16];
    uint32_t MouseCursorBufferAfter[16 * 16];
    bool MouseDrawn;
    uint32_t CurrentColour;
    uint64_t offset;
};

extern BasicRenderer basicRenderer;

#endif