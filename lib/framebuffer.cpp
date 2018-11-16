#include "framebuffer.h"

#include <algorithm>

PixelStyle oppositeStyle(PixelStyle style)
{
	PixelStyle opposite{PixelStyle::None};
	switch(style) {
		case PixelStyle::Set:
			opposite = PixelStyle::Unset;
			break;
		case PixelStyle::Unset:
			opposite = PixelStyle::Set;
			break;
		case PixelStyle::Toggle:
		case PixelStyle::None:
			opposite = PixelStyle::Toggle;
		default:
			opposite = PixelStyle::None;
	}

	return opposite;
}

Framebuffer::Framebuffer(int width, int height)
	: _width(width)
	, _height(height)
{

}

Framebuffer::~Framebuffer() = default;

void Framebuffer::clear()
{
	fillWith(0x00);
}

void Framebuffer::fill()
{
	fillWith(0xFF);
}

void Framebuffer::setPixel(int x, int y, PixelStyle style)
{
	switch(style) {
		case PixelStyle::Set:
			setPixel(x, y);
			break;
		case PixelStyle::Unset:
			unsetPixel(x, y);
			break;
		case PixelStyle::Toggle:
			togglePixel(x, y);
			break;
		case PixelStyle::None:
		default:
			break;
	}
}

void Framebuffer::setPixel(Pixel p)
{
	setPixel(p.x(), p.y());
}

void Framebuffer::setPixel(Pixel p, PixelStyle style)
{
	setPixel(p.x(), p.y(), style);
}

void Framebuffer::unsetPixel(Pixel p)
{
	unsetPixel(p.x(), p.y());
}

void Framebuffer::togglePixel(Pixel p)
{
	togglePixel(p.x(), p.y());
}

bool Framebuffer::isPixelSet(Pixel p) const
{
	return isPixelSet(p.x(), p.y());
}

bool Framebuffer::isPixelInside(int x, int y) const
{
	return (x >= 0) && (x < width()) && (y >= 0) && (y < height());
}

bool Framebuffer::isPixelInside(Pixel p) const
{
	return isPixelInside(p.x(), p.y());
}

void Framebuffer::setBuffer(const Framebuffer &bitmap, Pixel offset)
{
	setBuffer(bitmap, offset.x(), offset.y());
}

void Framebuffer::setBuffer(const Framebuffer &bitmap, int offsetX, int offsetY)
{
	int xStart = std::max(0, offsetX);
	int xEnd = std::min(width(), offsetX + bitmap.width());
	int yStart = std::max(0, offsetY);
	int yEnd = std::min(height(), offsetY + bitmap.height());

	for(int y = yStart; y < yEnd; ++y) {
		for(int x = xStart; x < xEnd; ++x) {
			if(bitmap.isPixelSet(x - offsetX, y - offsetY)) {
				setPixel(x, y);
			} else {
				unsetPixel(x, y);
			}
		}
	}
}

void Framebuffer::setBuffer(const Framebuffer &bitmap, PixelStyle style, Pixel offset)
{
	setBuffer(bitmap, style, offset.x(), offset.y());
}

void Framebuffer::setBuffer(const Framebuffer &bitmap, PixelStyle style, int offsetX, int offsetY)
{
	PixelStyle opposite = oppositeStyle(style);
	int xStart = std::max(0, offsetX);
	int xEnd = std::min(width(), offsetX + bitmap.width());
	int yStart = std::max(0, offsetY);
	int yEnd = std::min(height(), offsetY + bitmap.height());

	for(int y = yStart; y < yEnd; ++y) {
		for(int x = xStart; x < xEnd; ++x) {
			if(bitmap.isPixelSet(x - offsetX, y - offsetY)) {
				setPixel(x, y, style);
			} else {
				setPixel(x, y, opposite);
			}
		}
	}
}

void Framebuffer::resize(int width, int height)
{
	this->_width	= width;
	this->_height	= height;
}

int Framebuffer::width() const
{
	return this->_width;
}

int Framebuffer::height() const
{
	return this->_height;
}