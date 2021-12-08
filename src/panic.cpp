#include "panic.h"
#include "kernel.h"

void panic(const char *debug)
{
    basicRenderer.clear(RED);
    basicRenderer.Println("/!\\ KERNEL PANIC /!\\", BLACK);
    basicRenderer.Println(debug, BLACK);
}

void add_line(const char *debug)
{
    basicRenderer.Print(debug, BLACK);
}

void add_line_ln(const char *debug)
{
    basicRenderer.Println(debug, BLACK);
}