#include "graphics.h"

#include <algorithm>

void drawHorizontalLine(int y, int x1, int x2, PixelStyle style, Framebuffer &frame)
{
	if(x2 < x1)
		std::swap(x1, x2);
	int x = x1;

	frame.setPixel(Pixel(x, y), style);

	while(x != x2) {
		++x;
		frame.setPixel(Pixel(x, y), style);
	}
}

void drawVerticalLine(int x, int y1, int y2, PixelStyle style, Framebuffer &frame)
{
	if(y2 < y1)
		std::swap(y1, y2);
	int y = y1;

	frame.setPixel(Pixel(x, y), style);

	while(y != y2) {
		++y;
		frame.setPixel(Pixel(x, y), style);
	}
}

void drawLine(const Pixel &p1, const Pixel &p2, PixelStyle style, Framebuffer &frame)
{
	if(p1.y() == p2.y()) {
		drawHorizontalLine(p1.y(), p1.x(), p2.x(), style, frame);
	} else if(p1.x() == p2.x()) {
		drawVerticalLine(p1.x(), p1.y(), p2.y(), style, frame);
	} else {
		int dx = std::abs(p1.x() - p2.x());
		int dy = std::abs(p1.y() - p2.y());

		int sx = p1.x() < p2.x() ? 1 : -1;
		int sy = p1.y() < p2.y() ? 1 : -1;

		int err = (dx > dy ? dx : -dy) / 2;
		int err2;

		int x0 = p1.x();
		int y0 = p1.y();
		for(;;) {
			frame.setPixel(Pixel(x0, y0), style);
			if(x0 == p2.x() && y0 == p2.y())
				break;
			err2 = err;
			if(err2 > -dx) {
				err -= dy;
				x0 += sx;
			}
			if(err2 < dy) {
				err += dx;
				y0 += sy;
			}
		}
	}
}

void drawRectangle(const Pixel &p1, const Pixel &p2, PixelStyle style, Framebuffer &frame, bool filled)
{
	if(filled) {
		int x0 = std::min(p1.x(), p2.x());
		int x1 = std::max(p1.x(), p2.x());
		int y0 = std::min(p1.y(), p2.y());
		int y1 = std::max(p1.y(), p2.y());

		drawHorizontalLine(y0, x0, x1, style, frame);
		while(y0 != y1) {
			++y0;
			drawHorizontalLine(y0, x0, x1, style, frame);
		}
	} else {
		drawHorizontalLine(p1.y(), p1.x(), p2.x(), style, frame);
		drawHorizontalLine(p2.y(), p1.x(), p2.x(), style, frame);
		drawVerticalLine(p1.x(), p1.y(), p2.y(), style, frame);
		drawVerticalLine(p2.x(), p1.y(), p2.y(), style, frame);
	}
}

void drawCircle(const Pixel &center, int radius, PixelStyle style, Framebuffer &frame)
{
	int f = 1 - radius;
	int ddF_x = 0;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

	frame.setPixel(Pixel(center.x(), center.y() + radius), style);
	frame.setPixel(Pixel(center.x(), center.y() - radius), style);
	frame.setPixel(Pixel(center.x() + radius, center.y()), style);
	frame.setPixel(Pixel(center.x() - radius, center.y()), style);

	while(x < y) {
		if(f >= 0) {
			--y;
			ddF_y += 2;
			f += ddF_y;
		}
		++x;
		ddF_x += 2;
		f += ddF_x + 1;
		frame.setPixel(Pixel(center.x() + x, center.y() + y), style);
		frame.setPixel(Pixel(center.x() - x, center.y() + y), style);
		frame.setPixel(Pixel(center.x() + x, center.y() - y), style);
		frame.setPixel(Pixel(center.x() - x, center.y() - y), style);
		frame.setPixel(Pixel(center.x() + y, center.y() + x), style);
		frame.setPixel(Pixel(center.x() - y, center.y() + x), style);
		frame.setPixel(Pixel(center.x() + y, center.y() - x), style);
		frame.setPixel(Pixel(center.x() - y, center.y() - x), style);
	}
}