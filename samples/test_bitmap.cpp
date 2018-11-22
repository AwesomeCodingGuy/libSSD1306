#include <iostream>
#include <thread>
#include <chrono>

#include "oled.h"
#include "bitmap.h"

#define OLED_ADDR 0x3C

int main(int argc, char **argv) 
{
	try {
        Oled_128x64 oled("/dev/i2c-1", OLED_ADDR);

        std::cout << "Loading bitmap in byte format..." << std::endl;
        // Create an empty bitmap. The size does not matter here.
        Bitmap bmp(10, 10);
        // Load a bitmap from file. The size will be adjusted.
        bmp.load("byteSplash.bitmap");
        // Load the bitmap to the oleds framebuffer, with an Offset of (0|0) px.
        oled.setBuffer(bmp, 0, 0);
        // Send the buffer to the display.
        oled.displayUpdate();

        std::cout << "Watch the bitmap on the display. It will disappear in 5 seconds." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        oled.clear();
        oled.displayUpdate();
        std::this_thread::sleep_for(std::chrono::seconds(5));

        std::cout << "Now loading the same bitmap in bit format...we also reverse the bitmap when setting the buffer." << std::endl;
        Bitmap bmp2(10, 10);
        bmp2.load("bitSplash.bitmap");
        oled.setBuffer(bmp2, PixelStyle::Unset, 0, 0);
        oled.displayUpdate();

        std::cout << "The image is the same, however it requires much less disc space. It will also disappear in 5 seconds." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));

        oled.clear();
        oled.displayUpdate();
	} catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

    return EXIT_SUCCESS;
}