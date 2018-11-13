#include "bitmap.h"

#include <array>
#include <fstream>
#include <algorithm>	// std::fill

constexpr uint32_t magicFileNumber	= 0x4F4C4544;
constexpr uint8_t headerEndByte		= 0xDD;
constexpr uint8_t bitFormat			= 0x00;
constexpr uint8_t byteFormat		= 0x01;

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

bool Bitmap::load(const std::string &filename)
{
	std::ifstream inFile(filename, std::ifstream::binary);
	if(inFile.is_open()) {
		std::array<uint8_t, 16> header = {0x00};
		inFile.read((char *)&header[0], 16);
		if(inFile) {
			uint32_t startBytes = header[0] << 24 | header[1] << 16 | header[2] << 8 | header[3];
			uint8_t endByte = header[15];
			if(startBytes == magicFileNumber && endByte == headerEndByte) {
				uint16_t width = (header[4] << 8) | header[5];
				uint16_t height = (header[6] << 8) | header[7];
				uint8_t fmt = header[8];
				int bytesPerRow = 0;
				if(fmt == bitFormat) {
					bytesPerRow = (width + 7) / 8;
				} else if(fmt == byteFormat) {
					bytesPerRow = width;
				}

				inFile.seekg(0, inFile.end);
				int length = inFile.tellg();
				inFile.seekg(16, inFile.beg);
				if(length != 16 + bytesPerRow * height) {
					std::string what("Invalid file length " + filename + " " __FILE__ " (" + std::to_string(__LINE__) + ")");
					throw std::system_error(errno, std::system_category(), what);
				}

				this->_height = height;
				this->_width = width;
				this->_bytesPerRow = _bytesPerRow;
				this->_data.clear();
				this->_data.resize(this->_height, std::vector<uint8_t>(this->_bytesPerRow, 0x00));
				for(int h = 0; h < height; ++h) {
					inFile.read((char*)&(this->_data[h])[0], this->_bytesPerRow);
				}
			} else {
				std::string what("Invalid file format " + filename + " " __FILE__ " (" + std::to_string(__LINE__) + ")");
				throw std::system_error(errno, std::system_category(), what);
			}
		} else {
			std::string what("Read " __FILE__ " (" + std::to_string(__LINE__) + ")");
			throw std::system_error(errno, std::system_category(), what);
		}
	} else {
		std::string what("Open " + filename + " " __FILE__ " (" + std::to_string(__LINE__) + ")");
		throw std::system_error(errno, std::system_category(), what);
	}

	return false;
}

bool save(const std::string &filename)
{
	return false;
}