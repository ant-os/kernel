#pragma once
#include <stddef.h>

typedef struct header {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} PSF1_HEADER;

typedef struct font {
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
} PSF1_FONT;