#ifndef CLOCK_H
#define CLOCK_H

#include <ctime>

#include "framebuffer.h"

class Clock_64x64
{
	static constexpr int _width				= 64;
	static constexpr int _height			= 64;
	static constexpr int _hourHandLength	= 18;
	static constexpr int _minuteHandLength	= 26;
	static constexpr int _secondHandLength	= 30;
	static constexpr int _tickRadius		= 31;

public:
	Clock_64x64() = default;

	void draw(Framebuffer &frame, const Pixel &offset, time_t time);
	void drawNow(Framebuffer &frame, const Pixel &offset);

private:
	void drawHand(Framebuffer &frame, const Pixel &offset, float angle, int length);
};

#endif // CLOCK_H