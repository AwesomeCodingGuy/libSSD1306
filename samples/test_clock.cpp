#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>

#include "oled.h"
#include "complex/clock.h"

#define OLED_ADDR 0x3C

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
        Clock_64x64 clock;

        while(!shutdown) {
            clock.drawNow(oled, Pixel(32, 0));
            oled.displayUpdate();

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        oled.clear();
        oled.displayUpdate();
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}