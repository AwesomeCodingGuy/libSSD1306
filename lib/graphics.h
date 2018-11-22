#ifndef LIBSSD1306_GRAPHICS_H
#define LIBSSD1306_GRAPHICS_H

#include "pixel.h"
#include "framebuffer.h"

void drawHorizontalLine(Framebuffer &frame, int y, int x1, int x2, PixelStyle style);
void drawVerticalLine(Framebuffer &frame, int x, int y1, int y2, PixelStyle style);
void drawLine(Framebuffer &frame, const Pixel &p1, const Pixel &p2, PixelStyle style);
void drawRectangle(Framebuffer &frame, const Pixel &p1, const Pixel &p2, PixelStyle style, bool filled = false);
void drawCircle(Framebuffer &frame, const Pixel &center, int radius, PixelStyle style);

#endif // LIBSSD1306_GRAPHICS_H