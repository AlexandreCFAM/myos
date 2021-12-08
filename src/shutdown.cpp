#include "shutdown.h"

void shutdown()
{
    basicRenderer.Logln("Trying to shutdown your machine with IO ports (if you're running the kernel in a VM)...");
    
    basicRenderer.Logln("Qemu method...");
    out16(0xB004, 0x2000); //old version
    out16(0x604, 0x2000); //new version

    basicRenderer.Logln("VirtualBox method...");
    out16(0x4004, 0x3400);
}