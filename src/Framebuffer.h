#pragma once
#include <stddef.h>

typedef unsigned int color;

struct Framebuffer {
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine;
};