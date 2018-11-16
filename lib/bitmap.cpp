#include "bitmap.h"

#include <array>
#include <cstring>
#include <fstream>
#include <algorithm>	// std::fill

union MagicNumber_t {
	uint8_t bytes[4];
	uint32_t number;
};

constexpr MagicNumber_t magicFileNumber	= { 0x4F, 0x4C, 0x45, 0x44 };
constexpr uint8_t headerEndByte			= 0xDD;
constexpr std::array<uint8_t, 16> defaultHeader = 
{
	magicFileNumber.bytes[0], magicFileNumber.bytes[1], 
	magicFileNumber.bytes[2], magicFileNumber.bytes[3], 
	0x00, 0x00, 0x00, 0x00,
	static_cast<uint8_t>(Bitmap::Format::Bit), 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, headerEndByte
};

Bitmap::Bitmap(int width, int height)
	: Framebuffer(width, height)
	, _bytesPerRow((width + 7) / 8)
{
	_data.clear();
	_data.resize(this->height(), std::vector<uint8_t>(_bytesPerRow, 0x00));
}

Bitmap::Bitmap(int width, int height, std::vector<uint8_t> data)
	: Bitmap(width, height)
{
	for(int h = 0; h < height; ++h) {
		std::copy_n(data.begin() + (_bytesPerRow * h), _bytesPerRow, _data[h].begin());
	}
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
			if((memcmp(&header, &magicFileNumber.bytes, 4) == 0) && header[15] == headerEndByte) {
				uint16_t newWidth = (header[4] << 8) | header[5];
				uint16_t newHeight = (header[6] << 8) | header[7];
				Format fmt = static_cast<Format>(header[8]);
				int bytesPerRowRead = 0;
				if(fmt == Format::Bit) {
					bytesPerRowRead = (newWidth + 7) / 8;
				} else if(fmt == Format::Byte) {
					bytesPerRowRead = newWidth;
				}

				inFile.seekg(0, inFile.end);
				int length = inFile.tellg();
				inFile.seekg(16, inFile.beg);
				if(length != 16 + bytesPerRowRead * newHeight) {
					std::string what("Invalid file length " + filename + " " __FILE__ " (" + std::to_string(__LINE__) + ")");
					throw std::system_error(errno, std::system_category(), what);
				}

				this->resize(newWidth, newHeight);
				this->_bytesPerRow = (this->width() + 7) / 8;
				this->_data.clear();
				this->_data.resize(this->height(), std::vector<uint8_t>(this->_bytesPerRow, 0x00));
				if(fmt == Format::Bit) {
					for(int h = 0; h < this->height(); ++h) {
						inFile.read((char*)&(this->_data[h])[0], this->_bytesPerRow);
					}
				} else if(fmt == Format::Byte) {
					char c;
					for(int h = 0; h < this->height(); ++h) {
						for(int w = 0; w < this->width(); ++w) {
							c = inFile.get();
							if(c != 0x00) {
								setPixel(w, h);
							} else {
								unsetPixel(w, h);
							}
						}
					}
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
	inFile.close();

	return true;
}

bool Bitmap::save(const std::string &filename, Format fmt)
{
	std::ofstream outFile(filename, std::ofstream::binary);
	if(outFile.is_open()) {
		std::array<uint8_t, 16> header{defaultHeader};
		header[4] = (this->width() & 0xFF00) >> 8;
		header[5] = (this->width() & 0xFF);
		header[6] = (this->height() & 0xFF00) >> 8;
		header[7] = (this->height() & 0xFF);
		header[8] = static_cast<uint8_t>(fmt);
		outFile.write((char*)&header[0], 16);
		if(fmt == Format::Bit) {
			for(int h = 0; h < this->height(); ++h) {
				outFile.write((char*)_data[h].data(), _data[h].size());
			}
		} else if(fmt == Format::Byte) {
			for(int h = 0; h < this->height(); ++h) {
				for(int w = 0; w < this->width(); ++w) {
					outFile.put(isPixelSet(w, h) ? 0xFF : 0x00);
				}
			}
		}
	} else {
		std::string what("Open " + filename + " " __FILE__ " (" + std::to_string(__LINE__) + ")");
		throw std::system_error(errno, std::system_category(), what);
	}
	outFile.close();

	return false;
}