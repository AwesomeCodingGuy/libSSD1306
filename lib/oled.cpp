#include "oled.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <linux/i2c-dev.h>
#include <system_error>

constexpr uint8_t COMMAND								= 0x00;
constexpr uint8_t DATA									= 0x40;

// Address modes
constexpr uint8_t HORIZONTAL_ADDRESSING_MODE			= 0x00;
constexpr uint8_t VERTICAL_ADDRESSING_MODE				= 0x01;
constexpr uint8_t PAGE_ADDRESSING_MODE					= 0x02;

// Commands
constexpr uint8_t SET_COLUMN_START_LOW_MASK				= 0x00;
constexpr uint8_t SET_COLUMN_START_HIGH_MASK			= 0x10;
constexpr uint8_t SET_MEMORY_ADDRESSING_MODE			= 0x20;
constexpr uint8_t SET_COLUMN_ADDRESS					= 0x21;
constexpr uint8_t SET_PAGE_ADDRESS						= 0x22;
constexpr uint8_t SET_DISPLAY_START_LINE_MASK			= 0x40;
constexpr uint8_t SET_CONTRAST							= 0x81;
constexpr uint8_t ENABLE_CHARGE_PUMP_REGULATOR			= 0x8D;
constexpr uint8_t SET_SEGMENT_REMAP_0					= 0xA0;
constexpr uint8_t SET_SEGMENT_REMAP_127					= 0xA1;
constexpr uint8_t SET_ENTIRE_DISPLAY_ON_RESUME			= 0xA4;
constexpr uint8_t SET_ENTIRE_DISPLAY_ON_FORCE			= 0xA5;
constexpr uint8_t SET_NORMAL_DISPLAY					= 0xA6;
constexpr uint8_t SET_INVERSE_DISPLAY					= 0xA7;
constexpr uint8_t SET_MULTIPLEX_RATIO					= 0xA8;
constexpr uint8_t SET_DISPLAY_OFF						= 0xAE;
constexpr uint8_t SET_DISPLAY_ON						= 0xAF;
constexpr uint8_t SET_PAGE_START_ADDRESS_MASK			= 0xB0;
constexpr uint8_t SET_SCAN_DIRECTION_NORMAL				= 0xC0;
constexpr uint8_t SET_SCAN_DIRECTION_REMAP				= 0xC8;
constexpr uint8_t SET_DISPLAY_OFFSET					= 0xD3;
constexpr uint8_t SET_OSCILLATOR_FREQUENCY				= 0xD5;
constexpr uint8_t SET_PRECHARGE_PERIODE					= 0xD9;
constexpr uint8_t SET_COM_PINS_HARDWARE_CONFIGURATION	= 0xDA;
constexpr uint8_t SET_VCOMH_DESELECT_LEVEL				= 0xDB;


Oled_128x64::Oled_128x64(const std::string& device, uint8_t address)
	: Framebuffer(_width, _height)
	, _fd(-1)
{
	this->_fd = open(device.c_str(), O_RDWR);
	if(this->_fd < 0) {
		std::string what("Open " + device + " " __FILE__ " (" + std::to_string(__LINE__) + ")");
		throw std::system_error(errno, std::system_category(), what);
	}

	if(ioctl(this->_fd, I2C_SLAVE, address) < 0) {
		std::string what("iotcl I2C_SLAVE " __FILE__ " (" + std::to_string(__LINE__) + ")");
		throw std::system_error(errno, std::system_category(), what);
	}

	init();
}

void Oled_128x64::setPixel(int x, int y)
{
	if(!isPixelInside(x, y))
		return;

	PixelOffset o{x, y};

	if((this->_data[o.block].bytes[o.byte] & (1 << o.bit)) == 0) {
		this->_data[o.block].bytes[o.byte] |= (1 << o.bit);
		if(!this->_data[o.block].dirty)
			this->_data[o.block].dirty = true;
	}
}

void Oled_128x64::unsetPixel(int x, int y)
{
	if(!isPixelInside(x, y))
		return;

	PixelOffset o{x, y};

	if((this->_data[o.block].bytes[o.byte] & (1 << o.bit)) != 0) {
		this->_data[o.block].bytes[o.byte] &= ~(1 << o.bit);
		if(!this->_data[o.block].dirty)
			this->_data[o.block].dirty = true;
	}
}

void Oled_128x64::togglePixel(int x, int y)
{
	if(!isPixelInside(x, y))
		return;

	PixelOffset o{x, y};

	if((this->_data[o.block].bytes[o.byte] & (1 << o.bit)) == 0) {
		this->_data[o.block].bytes[o.byte] |= (1 << o.bit);
	} else {
		this->_data[o.block].bytes[o.byte] &= ~(1 << o.bit);
	}

	if(!this->_data[o.block].dirty) {
		this->_data[o.block].dirty = true;
	}
}

bool Oled_128x64::isPixelSet(int x, int y) const
{
	if(!isPixelInside(x, y))
		return false;

	PixelOffset o{x, y};

	return this->_data[o.block].bytes[o.byte] & (1 << o.bit);
}

void Oled_128x64::fillWith(uint8_t value)
{
	for(auto &block : this->_data) {
		auto &bytes = block.bytes;
		for(auto byte = bytes.begin() + this->_dataOffset; byte != bytes.end(); ++byte) {
			if(*byte != value) {
				*byte = value;
				if(!block.dirty)
					block.dirty = true;
			}
		}
	}
}

void Oled_128x64::displayInverse() const
{
	sendCommand(SET_INVERSE_DISPLAY);
}

void Oled_128x64::displayNormal() const
{
	sendCommand(SET_NORMAL_DISPLAY);
}

void Oled_128x64::displayOff() const
{
	sendCommand(SET_DISPLAY_OFF);
}

void Oled_128x64::displayOn() const
{
	sendCommand(SET_DISPLAY_ON);
}

void Oled_128x64::displaySetContrast(uint8_t contrast) const
{
	sendCommand(SET_CONTRAST);
	sendCommand(contrast);
}

void Oled_128x64::displayUpdate()
{
	uint8_t page = 0;
	uint8_t column = 0;

	for(auto &block : this->_data) {
		if(block.dirty) {
			uint8_t columnLow = column & 0x0F;
			uint8_t columnHigh = (column >> 4) & 0x0F;

			sendCommand(SET_PAGE_START_ADDRESS_MASK | page);
			sendCommand(SET_COLUMN_START_LOW_MASK | columnLow);
			sendCommand(SET_COLUMN_START_HIGH_MASK | columnHigh);

			if(write(this->_fd, block.bytes.data(), block.bytes.size()) == -1) {
				std::string what("Write " __FILE__ "(" + std::to_string(__LINE__) + ")");
				throw std::system_error(errno, std::system_category(), what);
			}
			block.dirty = false;
		}

		column += _columnsPerBlock;

		if(column >= width()) {
			column = 0;
			page += 1;
		}
	}
}

void Oled_128x64::init() const
{
	// Enable charge pump regulator
	sendCommand(ENABLE_CHARGE_PUMP_REGULATOR, 0x14);
	// Set memory addressing mode
	sendCommand(SET_MEMORY_ADDRESSING_MODE, PAGE_ADDRESSING_MODE);
	// Set OSC frequency
	sendCommand(SET_OSCILLATOR_FREQUENCY, 0x80);
	// Set display offset
	sendCommand(SET_DISPLAY_OFFSET, 0x00);
	// Set display start line
	sendCommand(SET_DISPLAY_START_LINE_MASK, 0x00);
	// Set segment remap
	sendCommand(SET_SEGMENT_REMAP_127);
	// Set COM output scan direction
	sendCommand(SET_SCAN_DIRECTION_REMAP);
	// Set COM pins hardware configuration
	sendCommand(SET_COM_PINS_HARDWARE_CONFIGURATION, 0x12);
	// Set precharge periode
	sendCommand(SET_PRECHARGE_PERIODE, 0xF1);
	// SET Vcomh deselect level
	sendCommand(SET_VCOMH_DESELECT_LEVEL, 0x40);
	// Disable entire display on
	sendCommand(SET_ENTIRE_DISPLAY_ON_RESUME);
	// Set normal display
	sendCommand(SET_NORMAL_DISPLAY);
	// Set column address
	sendCommand(SET_COLUMN_ADDRESS, 0x00, 0x7F);
	// Set page address
	sendCommand(SET_PAGE_ADDRESS, 0x00, 0x07);
	// Set contrast control
	sendCommand(SET_CONTRAST, 0x7F);
	// Display on
	sendCommand(SET_DISPLAY_ON);

	usleep(10000);
}

void Oled_128x64::sendCommand(uint8_t command) const
{
	std::array<uint8_t, 2> data({COMMAND, command});

	if(write(this->_fd, data.data(), data.size()) == -1) {
		std::string what("Write " __FILE__ "(" + std::to_string(__LINE__) + ")");
		throw std::system_error(errno, std::system_category(), what);
	}
}

void Oled_128x64::sendCommand(uint8_t command, uint8_t value) const
{
	std::array<uint8_t, 3> data({COMMAND, command, value});

	if(write(this->_fd, data.data(), data.size()) == -1) {
		std::string what("Write " __FILE__ "(" + std::to_string(__LINE__) + ")");
		throw std::system_error(errno, std::system_category(), what);
	}
}

void Oled_128x64::sendCommand(uint8_t command, uint8_t value1, uint8_t value2) const
{
	std::array<uint8_t, 4> data({COMMAND, command, value1, value2});

	if(write(this->_fd, data.data(), data.size()) == -1) {
		std::string what("Write " __FILE__ "(" + std::to_string(__LINE__) + ")");
		throw std::system_error(errno, std::system_category(), what);
	}
}

Oled_128x64::PixelBlock::PixelBlock()
	: bytes{DATA, 0x00}
	, dirty{true}
{

}