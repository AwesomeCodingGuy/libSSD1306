#include "font.h"

#include <iostream>

#include <array>
#include <fstream>
#include <algorithm>

static constexpr std::array<uint8_t, 8> magicNumber = {
	0x4F, 0x4C, 0x45, 0x44, 0x42, 0x4D, 0x50, 0x31
};

Font::Font(const std::string &fontFile)
	: _charWidth(0)
	, _charHeight(0)
{
	load(fontFile);
}

Pixel Font::drawChar(Framebuffer &frame, uint8_t c, const Pixel &pos, PixelStyle style)
{
	frame.setBuffer(_font[c], style, pos);
	return Pixel(pos.x() + _charWidth, pos.y());
}

Pixel Font::drawString(Framebuffer &frame, const char *str, const Pixel &pos, PixelStyle style)
{
	Pixel p{pos};

	if(str != nullptr) {
		Pixel start{p};
		while(*str != '\0') {
			if(*str == '\n') {
				p.set(start.x(), p.y() + _charHeight);
			} else {
				drawChar(frame, *str, p, style);
				p.set(p.x() + _charWidth, p.y());
			}
			++str;
		}
	}

	return p;
}

Pixel Font::drawString(Framebuffer &frame, const std::string &str, const Pixel &pos, PixelStyle style)
{
	return drawString(frame, str.c_str(), pos, style);
}

bool Font::load(const std::string &fontFile)
{
	std::ifstream inFile(fontFile, std::ifstream::binary);
	if(inFile.is_open()) {
		std::array<uint8_t, 16> header = {0x00};
		inFile.read((char *)&header[0], 16);
		if(inFile) {
			if(std::equal(magicNumber.begin(), magicNumber.end(), header.begin())) {
				uint16_t totalChars = (header[8] << 8) | header[9];
				uint8_t charHeight	= header[10];
				uint8_t charWidth	= header[11];
				uint16_t dataHeight = (header[12] << 8) | header[13];
				uint16_t dataWidth  = (header[14] << 8) | header[15];

				inFile.seekg(0, inFile.end);
				int length = inFile.tellg();
				inFile.seekg(16, inFile.beg);
				if(length != (16 + dataHeight * dataWidth)) {
					std::string what("Invalid file length " + fontFile + " " __FILE__ " (" + std::to_string(__LINE__) + ")");
					throw std::system_error(errno, std::system_category(), what);
				}

				_charWidth = charWidth;
				_charHeight = charHeight;
				_font.clear();

				uint16_t charBytes = charHeight * ((charWidth + 7) / 8);
				std::vector<uint8_t> charBuffer(charBytes, 0x00);
				for(int c = 0; c < totalChars; ++c) {
					inFile.read((char *)&charBuffer[0], charBytes);
					if(!inFile) {
						std::string what("Read " __FILE__ " (" + std::to_string(__LINE__) + ")");
						throw std::system_error(errno, std::system_category(), what);
					} else {
						_font.push_back(Bitmap(charWidth, charHeight, charBuffer));
					}
				}
			} else {
				std::string what("Invalid file format " + fontFile + " " __FILE__ " (" + std::to_string(__LINE__) + ")");
				throw std::system_error(errno, std::system_category(), what);
			}
		} else {
			std::string what("Read " __FILE__ " (" + std::to_string(__LINE__) + ")");
			throw std::system_error(errno, std::system_category(), what);
		}
	} else {
		std::string what("Open " + fontFile + " " __FILE__ " (" + std::to_string(__LINE__) + ")");
		throw std::system_error(errno, std::system_category(), what);
	}
	inFile.close();

	return true;
}

Bitmap Font::getChar(uint8_t c)
{
	return _font[c];
}

std::vector<Bitmap> Font::getAllChars()
{
	return _font;
}