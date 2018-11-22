#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>

#include "oled.h"
#include "font.h"
#include "complex/clock.h"

#define OLED_ADDR 0x3C

#define FONT1 0
#define FONT2 1

volatile static std::sig_atomic_t shutdown = 0;

static void SignalHandler(int sig)
{
    if(sig == SIGINT || sig == SIGTERM) {
        shutdown = 1;
    }
}

int main(int argc, char **argv)
{
    try {
        if(std::signal(SIGINT, SignalHandler) == SIG_ERR) {
            std::cerr << "Failed to install signal handler for SIGINT." << std::endl;
            return EXIT_FAILURE;
        }
        if(std::signal(SIGTERM, SignalHandler) == SIG_ERR) {
            std::cerr << "Failed to install signal handler for SIGTERM." << std::endl;
            return EXIT_FAILURE;
        }

        Oled_128x64 oled("/dev/i2c-1", OLED_ADDR);
        Font font8x8("OledFont8x8.oledfont");
        Font font8x16("OledFont8x16.oledfont");

        int x = 0;
        int y = 0;
        unsigned char c = 0;
        int font = FONT1;
        while(!shutdown) {
            if(font == FONT1)
                font8x8.drawChar(oled, c, Pixel(x * 8, y * 8), PixelStyle::Set);
            if(font == FONT2)
                font8x16.drawChar(oled, c, Pixel(x * 8, y * 16), PixelStyle::Set);
            if(++x == 16) {
                x = 0;
                ++y;
            }
            if((font == FONT2 && y >= 4) || (font == FONT1 && y >= 8)) {
                y = 0;
                oled.clear();
            }
            if(++c == 0) {
                (font == FONT1) ? font = FONT2 : font = FONT1;
            }
            oled.displayUpdate();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        oled.clear();
        oled.displayUpdate();
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}