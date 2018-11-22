#ifndef LIBSSD1306_OLED_H
#define LIBSSD1306_OLED_H

#include <string>
#include <cstdint>
#include <array>

#include "framebuffer.h"

class Oled_128x64 : public Framebuffer
{
    static constexpr int _width             = 128;
    static constexpr int _height            = 64;
    static constexpr int _bytesPerBlock     = 32;
    static constexpr int _dataOffset        = 1;
    static constexpr int _bufferSize        = _bytesPerBlock + _dataOffset;
    static constexpr int _blocks            = (_width * _height) / (8 * _bytesPerBlock);
    static constexpr int _columnsPerBlock   = _bytesPerBlock;
    static constexpr int _columnsPerRow     = _width / _columnsPerBlock;

public:
    Oled_128x64(const std::string& device, uint8_t address);

    void setPixel(int x, int y) override;
    void unsetPixel(int x, int y) override;
    void togglePixel(int x, int y) override;
    bool isPixelSet(int x, int y) const override;
    void fillWith(uint8_t value) override;

    void displayInverse() const;
    void displayNormal() const;
    void displayOff() const;
    void displayOn() const;
    void displaySetContrast(uint8_t contrast) const;
    void displayUpdate();

private:
    void init() const;
    void sendCommand(uint8_t command) const;
    void sendCommand(uint8_t command, uint8_t value) const;
    void sendCommand(uint8_t command, uint8_t value1, uint8_t value2) const;

    struct PixelOffset
    {
        PixelOffset(int x, int y)
            : bit(y % 8)
            , byte(_dataOffset + (x % _columnsPerBlock))
            , block((x / _columnsPerBlock) + (_columnsPerRow * (y / 8)))
        {

        }

        int bit;
        int byte;
        int block;
    };

    struct PixelBlock
    {
        PixelBlock();
        std::array<uint8_t, _bufferSize> bytes;
        bool dirty;
    };
    std::array<PixelBlock, _blocks> _data;

    int _fd;
};

#endif // LIBSSD1306_OLED_H