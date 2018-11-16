#ifndef BITMAP_H
#define BITMAP_H

#include <vector>
#include <string>

#include "framebuffer.h"

class Bitmap : public Framebuffer
{
public:
	enum class Format : uint8_t
	{
		Bit		= 0x00,
		Byte	= 0x01
	};

	Bitmap() = delete;
	Bitmap(int width, int height);
	Bitmap(int width, int height, std::vector<uint8_t> data);

	void setPixel(int x, int y) override;
	void unsetPixel(int x, int y) override;
	void togglePixel(int x, int y) override;
	bool isPixelSet(int x, int y) const override;
	void fillWith(uint8_t value) override;

	bool load(const std::string &filename);
	bool save(const std::string &filename, Format fmt = Format::Bit);

private:

	struct PixelOffset
	{
		PixelOffset(int x, int y)
			: row(y)
			, byte(x / 8)
			, bit(7 - (x % 8))
		{

		}

		int row;
		int byte;
		int bit;
	};

	int _bytesPerRow;
	std::vector<std::vector<uint8_t>> _data;
};

#endif // BITMAP_H