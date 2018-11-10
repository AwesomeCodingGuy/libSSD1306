#ifndef PIXEL_H
#define PIXEL_H

class Pixel
{
public:
	Pixel(int x, int y)
		: _x(x)
		, _y(y)
	{

	}

	int x() const { return _x; }
	int y() const { return _y; }

	void set(int x, int y) {
		_x = x;
		_y = y;
	}

private:
	int _x;
	int _y;
};

#endif // PIXEL_H