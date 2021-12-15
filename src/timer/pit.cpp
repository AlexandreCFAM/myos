#include "pit.h"

struct ProgrammableIntervalTimer PIT;

void ProgrammableIntervalTimer::init(uint16_t divisor)
{
   this->Tick = 0;
   this->frequency = PIT_FREQUENCY / divisor;
   asm("cli");
   outb(0x40, this->frequency & 0xFF);
   io_wait();
   outb(0x40, (this->frequency & 0xFF00) >> 8);
   asm("sti");
   basicRenderer.Logln("Programmable Interval Interrupt initialized!");
}

void ProgrammableIntervalTimer::sleep(uint8_t n)
{
   uint64_t current = this->Tick;
   while((this->Tick - current) <= n * this->frequency) asm("hlt");
}

void ProgrammableIntervalTimer::sleep_milliseconds(uint16_t n)
{
   uint64_t current = this->Tick;
   while((this->Tick - current) <= (n * this->frequency)/1000) asm("hlt");
}
