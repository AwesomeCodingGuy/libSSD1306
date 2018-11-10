#include "bitmap.h"

#include <algorithm>	// std::fill

Bitmap::Bitmap(int width, int height)
	: Framebuffer(width, height)
	, _bytesPerRow((width + 7) / 8)
{
	_data.clear();
	_data.resize(this->height(), std::vector<uint8_t>(_bytesPerRow, 0x00));
}

void Bitmap::setPixel(int x, int y)
{
	if(!isPixelInside(x, y))
		return;
	
	PixelOffset o{x, y};

	_data[o.row][o.byte] |= (1 << o.bit);
}

void Bitmap::unsetPixel(int x, int y)
{
	if(!isPixelInside(x, y))
		return;

	PixelOffset o{x, y};

	_data[o.row][o.byte] &= ~(1 << o.bit);
}

void Bitmap::togglePixel(int x, int y)
{
	if(!isPixelInside(x, y))
		return;

	PixelOffset o{x, y};

	if(_data[o.row][o.byte] & (1 << o.bit)) {
		_data[o.row][o.byte] &= ~(1 << o.bit);
	} else {
		_data[o.row][o.byte] |= (1 << o.bit);
	}
}

bool Bitmap::isPixelSet(int x, int y) const
{
	if(!isPixelInside(x, y))
		return false;

	PixelOffset o{x, y};

	return _data[o.row][o.byte] & (1 << o.bit);
}

void Bitmap::fillWith(uint8_t value)
{
	for(std::vector<uint8_t> &vec : _data) {
		std::fill(vec.begin(), vec.end(), value);
	}
}

bool Bitmap::load(std::string filename)
{
	return false;
}

bool Bitmap::save(std::string filename)
{
	return false;
}