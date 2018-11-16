#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <cstdint>

#include "pixel.h"

enum class PixelStyle
{
	None,
	Unset,
	Set,
	Toggle
};

PixelStyle oppositeStyle(PixelStyle style);

class Framebuffer
{
public:
	Framebuffer() = delete;
	Framebuffer(int width, int height);
	virtual ~Framebuffer() = 0;

	virtual void clear();
	virtual void fill();
	virtual void setPixel(int x, int y) = 0;
	void setPixel(int x, int y, PixelStyle style);
	void setPixel(Pixel p);
	void setPixel(Pixel p, PixelStyle style);
	virtual void unsetPixel(int x, int y) = 0;
	void unsetPixel(Pixel p);
	virtual void togglePixel(int x, int y) = 0;
	void togglePixel(Pixel p);

	virtual bool isPixelSet(int x, int y) const = 0;
	bool isPixelSet(Pixel p) const;
	bool isPixelInside(int x, int y) const;
	bool isPixelInside(Pixel p) const;

	void setBuffer(const Framebuffer &bitmap, Pixel offset);
	void setBuffer(const Framebuffer &bitmap, int offsetX, int offsetY);
	void setBuffer(const Framebuffer &bitmap, PixelStyle style, Pixel offset);
	void setBuffer(const Framebuffer &bitmap, PixelStyle style, int offsetX, int offsetY);

	int width() const;
	int height() const;

protected:
	virtual void fillWith(uint8_t value) = 0;

	void resize(int width, int height);

private:
	int _width;
	int _height;
};

#endif // FRAMEBUFFER_H