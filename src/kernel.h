#ifndef __KERNEL__H
#define __KERNEL__H

#include "graphics/textrenderer.h"
#include "lib/cstr.h"
#include "memory/efiMemory.h"
#include "memory.h"
#include "memory/bitmap.h"
// #include "renderer.h"
#include "paging/frame_alloc.h"
// #include "paging/paging.h"
// #include "paging/PageMapIndexer.h"
// #include "paging/PageTableManager.h"
#include "gdt/gdt.h"
#include "interrupts/IDT.h"
#include "interrupts/interrupts.h"
#include "panic.h"
#include "hardware/IO.h"
#include "drivers/PS2/mouse.h"
#include "drivers/pci.h"
#include "lib/math.h"
#include "drivers/drivers.h"
#include "acpi/acpi.h"
#include "memory/manager.h"
#include <stdint.h>
#include "panic.h"
#include "cpu/registers.h"
#include "hardware/shutdown.h"
#include "timer/pit.h"
#include "drivers/keyboard/keyboard.h"
#include "cpu/cpu.h"

struct BootInfo
{
	FrameBuffer *framebuffer;
	PSF1_FONT *psf1_font;
	EFI_MEMORY_DESCRIPTOR *mMap;
	uint64_t mMapSize;
	uint64_t mMapDescriptorSize;
	void *rsdp;
};

struct KernelInfo
{
    uint64_t MemoryMapEntries;
	void *rsdp;
};

extern Point MainTextCursor;
extern TextRenderer basicRenderer;
extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;
extern BootInfo *bootInfo;
extern KernelInfo *kernelInfo;

void KernelShowBootInfos(BootInfo *bootInfo, uint64_t kernelSize, uint64_t MemoryMapEntries);

#endif