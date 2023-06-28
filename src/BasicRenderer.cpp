#include "BasicRenderer.h"
#include <ant/memory.h>

BasicRenderer* GlobalRenderer;

BasicRenderer::BasicRenderer(Framebuffer* targetFramebuffer, PSF1_FONT* psf1_Font)
{
    TargetFramebuffer = targetFramebuffer;
    PSF1_Font = psf1_Font;
    Colour = 0xffffffff;
    CursorPosition = { 0, 0 };
}

bool BasicRenderer::Print(const char* str)
{

    char* chr = (char*)str;
    while (*chr != 0) {
        if (*chr == '\n') {
            Next();

        }
        else {
            if (*chr == (-1)) return false;
            PutChar(*chr, CursorPosition.X, CursorPosition.Y);
            CursorPosition.X += 8;
            if (CursorPosition.X + 8 > TargetFramebuffer->Width)
            {
                CursorPosition.X = 0;
                CursorPosition.Y += 16;
            }
            chr++;
        }
    }

    return true;
}

void BasicRenderer::PutChar(char chr, unsigned int xOff, unsigned int yOff)
{


    if (chr == '\n') {
        Next();
        return;
    }

    unsigned int* pixPtr = (unsigned int*)TargetFramebuffer->BaseAddress;
    char* fontPtr = (char*)PSF1_Font->glyphBuffer + (chr * PSF1_Font->psf1_Header->charsize);
    for (unsigned long y = yOff; y < yOff + 16; y++) {
        for (unsigned long x = xOff; x < xOff + 8; x++) {
            if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0) {
                *(unsigned int*)(pixPtr + x + (y * TargetFramebuffer->PixelsPerScanLine)) = Colour;
            }

        }
        fontPtr++;
    }


}

void BasicRenderer::PutChar(char chr)
{

    PutChar(chr, CursorPosition.X, CursorPosition.Y);
    CursorPosition.X += 8;
    if (CursorPosition.X + 8 > TargetFramebuffer->Width)
    {
        CursorPosition.X = 0;
        CursorPosition.Y += 16;
    }

}

void BasicRenderer::Next()
{
    CursorPosition = {
        0,
        CursorPosition.Y + 16
    };
}

void BasicRenderer::Goto(Point pos)
{
    CursorPosition = pos;
}

void BasicRenderer::SetColor(color colour)
{
    Colour = colour;
}

color BasicRenderer::GetColor()
{
    return (color)Colour;
}

void BasicRenderer::Clear()
{
    memset((void*)TargetFramebuffer->BaseAddress, 0, TargetFramebuffer->BufferSize);
}

void BasicRenderer::SetPixel(Point pos, color colour)
{
    color* pixPtr = (color*)TargetFramebuffer->BaseAddress;
    *(color*)(pixPtr + pos.X + (pos.Y * TargetFramebuffer->PixelsPerScanLine)) = colour;
}

color BasicRenderer::GetPixel(Point pos)
{
    color* pixPtr = (color*)TargetFramebuffer->BaseAddress;
    return (*(color*)(pixPtr + pos.X + (pos.Y * TargetFramebuffer->PixelsPerScanLine)));


}

int abs(int x)
{
    return (x + (x >> 31)) ^ (x >> 31);
}

void BasicRenderer::DrawLine(Point p1, Point p2, color colour) {
    int x1, y1, x2, y2;
    x1 = p1.X;
    x2 = p2.X;
    y1 = p1.Y;
    y2 = p2.Y;
    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
    dx = x2 - x1; dy = y2 - y1;
    dx1 = abs(dx); dy1 = abs(dy);
    px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
    if (dy1 <= dx1)
    {
        if (dx >= 0)
        {
            x = x1; y = y1; xe = x2;
        }
        else
        {
            x = x2; y = y2; xe = x1;
        }

        SetPixel(Point{ X: x, Y : y }, colour);

        for (i = 0; x < xe; i++)
        {
            x = x + 1;
            if (px < 0)
                px = px + 2 * dy1;
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
                px = px + 2 * (dy1 - dx1);
            }
            SetPixel(Point{ X: x, Y : y }, colour);
        }
    }
    else
    {
        if (dy >= 0)
        {
            x = x1; y = y1; ye = y2;
        }
        else
        {
            x = x2; y = y2; ye = y1;
        }

        SetPixel(Point{ X: x, Y : y }, colour);

        for (i = 0; y < ye; i++)
        {
            y = y + 1;
            if (py <= 0)
                py = py + 2 * dx1;
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
                py = py + 2 * (dx1 - dy1);
            }
            SetPixel(Point{ X: x, Y : y }, colour);
        }
    }
}

void BasicRenderer::DrawRect(Point pos, int32_t w, int32_t h, color colour)
{
    int x, y;
    x = pos.X;
    y = pos.Y;

    DrawLine(Point{ X: x, Y : y }, Point{ X: x + w, Y : y }, colour);
    DrawLine(Point{ X: x + w, Y : y, }, Point{ X: x + w, Y : y + h }, colour);
    DrawLine(Point{ X: x + w, Y : y + h }, Point{ X: x, Y : y + h }, colour);
    DrawLine(Point{ X: x, Y : y + h }, Point{ x, y }, colour);
}

void BasicRenderer::FillRect(Point pos, int32_t w, int32_t h, color colour)
{
    long x, y;
    x = pos.X;
    y = pos.Y;
    long x2 = x + w;
    long y2 = y + h;

    /// TODO: Implement Cliping...

    for (int i = x; i < x2; i++)
        for (int j = y; j < y2; j++)
            SetPixel({i, j}, colour);
}

void BasicRenderer::DrawTriangle(Point p1, Point p2, Point p3, color colour)
{
    DrawLine(p1, p2, colour);
    DrawLine(p2, p3, colour);
    DrawLine(p3, p1, colour);
}

void BasicRenderer::DrawIcon(Point pos, Icon icon)
{
    int x2 = pos.X + icon.w;
    int y2 = pos.X + icon.w;

    for (int i = pos.X; i < x2; i++)
        for (int j = pos.Y; j < y2; j++)
            SetPixel(Point
                { X: i, Y : j }, icon.data[i + (j * 1)]);
}

unsigned int BasicRenderer::GetWidth()
{
    return this->TargetFramebuffer->Width;
}

unsigned int BasicRenderer::GetHeight()
{
    return this->TargetFramebuffer->Height;
}

unsigned int BasicRenderer::GetPixelsPerScanLine()
{
    return this->TargetFramebuffer->PixelsPerScanLine;
}

void BasicRenderer::ClearC(color colour)
{
    uint64_t fbBase = (uint64_t)TargetFramebuffer->BaseAddress;
    uint64_t bytesPerScanline = TargetFramebuffer->PixelsPerScanLine * 4;
    uint64_t fbHeight = TargetFramebuffer->Height;
    uint64_t fbSize = TargetFramebuffer->BufferSize;

    for (int verticalScanline = 0; verticalScanline < fbHeight; verticalScanline++)
    {
        uint64_t pixPtrBase = fbBase + (bytesPerScanline * verticalScanline);
        for (uint32_t* pixPtr = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)(pixPtrBase + bytesPerScanline); pixPtr++)
        {
            *pixPtr = (uint32_t)colour;
        }
    }
}

void BasicRenderer::ClearChar()
{
    if (CursorPosition.X == 0)
    {
        CursorPosition.X = TargetFramebuffer->Width;
        CursorPosition.Y -= 16;
        if (CursorPosition.Y < 0)
            CursorPosition.Y = 0;
    }

    unsigned int xOff = CursorPosition.X;
    unsigned int yOff = CursorPosition.Y;

    unsigned int* pixPtr = (unsigned int*)TargetFramebuffer->BaseAddress;
    for (unsigned long y = yOff; y < yOff + 16; y++)
    {
        for (unsigned long x = xOff - 8; x < xOff; x++)
        {
            *(unsigned int*)(pixPtr + x + (y * TargetFramebuffer->PixelsPerScanLine)) = 0;
        }
    }

    CursorPosition.X -= 8;

    if (CursorPosition.X < 0)
    {
        CursorPosition.X = TargetFramebuffer->Width;
        CursorPosition.Y -= 16;
        if (CursorPosition.Y < 0)
            CursorPosition.Y = 0;
    }
}

void BasicRenderer::ClearLine()
{
    for (size_t i = 0; i < GetHeight(); i++)
    {
        PutChar(' ',  i, CursorPosition.Y);
    }
    
    
}
