#include "clock.h"

#include <cmath>

#include "graphics.h"

void Clock_64x64::draw(Framebuffer &frame, const Pixel &offset, time_t time)
{
	struct tm* tm = localtime(&time);

	drawRectangle(frame, offset, Pixel(offset.x() + _width, offset.y() + _height), PixelStyle::Unset, true);

	float hour		= (tm->tm_hour % 12) / 12.0;
	float minute	= tm->tm_min / 60.0;
	float second	= tm->tm_sec / 60.0;

	drawHand(frame, offset, hour + (minute / 12.0), _hourHandLength);
	drawHand(frame, offset, minute + (second / 60.0), _minuteHandLength);
	drawHand(frame, offset, second, _secondHandLength);

	for(int tick = 0; tick < 12; ++tick) {
		float radians = tick * float(M_PI) / 6.0;
		frame.setPixel(Pixel(int(sinf(radians) * _tickRadius) + (_width / 2) + offset.x(),
							 int(cosf(radians) * _tickRadius) + (_height / 2) + offset.y()));
	}
}

void Clock_64x64::drawNow(Framebuffer &frame, const Pixel &offset)
{
	time_t now;
	time(&now);
	draw(frame, offset, now);
}

void Clock_64x64::drawHand(Framebuffer &frame, const Pixel &offset, float angle, int length)
{
	float radians = 2.f * angle * float(M_PI);

	int xCenter = (_width / 2) + offset.x();
	int yCenter = (_height / 2) + offset.y();

	drawLine(frame,
             Pixel(xCenter, yCenter),
			 Pixel(xCenter + int(sinf(radians) * length),
				   yCenter - int(cosf(radians) * length)),
			 PixelStyle::Set);
}