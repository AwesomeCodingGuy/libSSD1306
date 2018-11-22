#include <iostream>
#include <thread>
#include <chrono>

#include "font.h"
#include "oled.h"
#include "bitmap.h"
#include "graphics.h"
#include "complex/clock.h"

#define OLED_ADDR	0x3C

int main(int argc, char **argv)
{
    std::string fontfile = "/var/tmp/src/libSSD1306/Pi-Debug/lib/fonts/OledFont8x16.oledfont";
    Font font(fontfile);
    Oled_128x64 oled("/dev/i2c-1", OLED_ADDR);

    Clock_64x64 clock;

    for(int i = 0; i < 180; ++i) {
        clock.drawNow(oled, Pixel(64, 0));
        oled.displayUpdate();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    oled.clear();
    oled.displayUpdate();


#if 0

    char bytes[4][8*17];
    for(int a = 0; a < 4; ++a) {
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 16; ++j) {
                bytes[a][i * 17 + j] = (char)(uint8_t((i + 4*a) * 16 + j));
            }
            bytes[a][i * 17 + 16] = '\n';
        }
    }

    bytes[0][0] = 32;
    bytes[0][10] = 32;
    std::cout << "Part 1" << std::endl;
    font.drawString(oled, bytes[0], Pixel(0,0), PixelStyle::Set);
    oled.setBuffer(font.getChar(10), Pixel(10*8, 0));
    oled.displayUpdate();
    getchar();

    std::cout << "Part 2" << std::endl;
    font.drawString(oled, bytes[1], Pixel(0, 0), PixelStyle::Set);
    oled.displayUpdate();
    getchar();

    std::cout << "Part 3" << std::endl;
    font.drawString(oled, bytes[2], Pixel(0, 0), PixelStyle::Set);
    oled.displayUpdate();
    getchar();

    std::cout << "Part 4" << std::endl;
    font.drawString(oled, bytes[3], Pixel(0, 0), PixelStyle::Set);
    oled.displayUpdate();

//
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

    std::string file	= "/var/tmp/src/libSSD1306/Pi-Debug/samples/bitSplash.bitmap";
    std::string file3	= "/var/tmp/src/libSSD1306/Pi-Debug/samples/bitSplash2.bitmap";

    Bitmap b(128, 64);
    b.load(file);
    b.save(file3, Bitmap::Format::Bit);
    oled.setBuffer(b, 0, 0);
    oled.displayUpdate();
    getchar();



    std::string file2 = "/var/tmp/src/libSSD1306/Pi-Debug/samples/byteSplash.bitmap";
    std::string file4 = "/var/tmp/src/libSSD1306/Pi-Debug/samples/byteSplash2.bitmap";

    Bitmap b2(10, 10);
    b2.load(file2);
    b2.save(file4, Bitmap::Format::Byte);
    oled.setBuffer(b2, 0, 0);

    drawCircle(Pixel(32, 32), 30, PixelStyle::Toggle, oled);
    drawCircle(Pixel(96, 32), 15, PixelStyle::Toggle, oled);
    
    
    oled.displayInverse();


    oled.displayUpdate();
#endif

    getchar();
    return 0;
}