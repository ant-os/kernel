#pragma once
#include "math.h"
#include "Framebuffer.h"
#include "simpleFonts.h"
#include "memory.h"
#include "cstr.h"

struct Icon{
    int w;
    int h;
    color* data;
};

class BasicRenderer{
    public:
    BasicRenderer(Framebuffer* targetFramebuffer, PSF1_FONT* psf1_Font);
    Point CursorPosition;
    Framebuffer* TargetFramebuffer;
    PSF1_FONT* PSF1_Font;
    unsigned int Colour;
    bool Print(const char* str);
    void PutChar(char chr, unsigned int xOff, unsigned int yOff);
    void PutChar(char chr);
    void Next();
    void Goto(Point pos);
    void SetColor(color colour);
    color GetColor();
    void Clear();
    void SetPixel(Point pos, color colour);
    color GetPixel(Point pos);
    void DrawLine(Point p1, Point p2, color colour);
    void DrawRect(Point pos, int32_t w, int32_t h, color colour = 0xffffffff);
    void FillRect(Point pos, int32_t w, int32_t h, color colour = 0xffffffff);
    void DrawTriangle(Point p1, Point p2, Point p3, color colour = 0xffffffff);
    void DrawIcon(Point pos, Icon icon);
    unsigned int GetWidth();
    unsigned int GetHeight();
    unsigned int GetPixelsPerScanLine();
    void ClearC(color colour);
    void ClearChar();
};

#define CENTER(rp)                                               \
    Point                                                        \
    {                                                            \
    X:                                                           \
        (int)(rp->GetWidth() / 2), Y : (int)(rp->GetHeight() / 2) \
    }

#define DIMENSIONS(rp)                                                \
        (int)(rp->GetWidth()), (int)(rp->GetHeight())

#define SCR_END(rp)                                               \
    Point                                                         \
    {                                                             \
    X:                                                            \
        (int)(rp->GetWidth()), Y : (int)(rp->GetHeight()) \
    }

#define ICON(w, h, d) Icon{w: w, h: h, data: d}

extern BasicRenderer* GlobalRenderer;