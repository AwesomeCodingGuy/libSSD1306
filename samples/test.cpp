#include <iostream>

#include "oled.h"
#include "bitmap.h"
#include "graphics.h"

#define OLED_ADDR	0x3C

int main(int argc, char **argv)
{
	Bitmap buffer = Bitmap(50, 50);
	buffer.setPixel(34, 34);
	Oled_128x64 oled("/dev/i2c-1", OLED_ADDR);
	oled.setPixel(34, 34);

	oled.displayUpdate();

	std::cout << "H: " << oled.height() << std::endl;
	std::cout << "W: " << oled.width() << std::endl;

	std::cout << (oled.isPixelSet(34, 34) ? "ja" : "nein") << std::endl;
	std::cout << (buffer.isPixelSet(34, 34) ? "ja" : "nein") << std::endl;

	oled.displayUpdate();
	getchar();
	drawCircle(Pixel(32, 32), 30, PixelStyle::Set, oled);
	drawCircle(Pixel(96, 32), 15, PixelStyle::Set, oled);
	oled.displayUpdate();

	getchar();
	oled.clear();
	oled.displayUpdate();

	return 0;
}