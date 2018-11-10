#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "pixel.h"
#include "framebuffer.h"

void drawHorizontalLine(int y, int x1, int x2, PixelStyle style, Framebuffer &frame);
void drawVerticalLine(int x, int y1, int y2, PixelStyle style, Framebuffer &frame);
void drawLine(const Pixel &p1, const Pixel &p2, PixelStyle style, Framebuffer &frame);
void drawRectangle(const Pixel &p1, const Pixel &p2, PixelStyle style, Framebuffer &frame, bool filled = false);
void drawCircle(const Pixel &center, int radius, PixelStyle style, Framebuffer &frame);

#endif // GRAPHICS_H