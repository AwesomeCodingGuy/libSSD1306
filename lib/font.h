#ifndef FONT_H
#define FONT_H

#include <vector>

#include "pixel.h"
#include "bitmap.h"

class Font
{
public:
	Font() = delete;
	Font(const std::string &fontFile);

	Pixel drawChar(Framebuffer &frame, uint8_t c, const Pixel &pos, PixelStyle style);
	Pixel drawString(Framebuffer &frame, const char *str, const Pixel &pos, PixelStyle style);
	Pixel drawString(Framebuffer &frame, const std::string &str, const Pixel &pos, PixelStyle style);

	Bitmap getChar(uint8_t c);
	std::vector<Bitmap> getAllChars();

private:
	bool load(const std::string &fontFile);

	std::vector<Bitmap> _font;
	int _charWidth;
	int _charHeight;
};

#endif // FONT_H