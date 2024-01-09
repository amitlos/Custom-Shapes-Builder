#include "Segment.h"
#include <math.h>

Point Segment::getPointOn(char name, unsigned int p1, unsigned int p2) const
{
	unsigned int fromMinX = abs(((long long)_p2._x - (long long)_p1._x)) * p1 / (p1 + p2);
	unsigned int fromMinY = abs((long long)_p2._y - (long long)_p1._y) * p1 / (p1 + p2);
	unsigned int x = (_p1._x > _p2._x ? _p2._x : _p1._x) + fromMinX;
	unsigned int y = (_p1._y > _p2._y ? _p2._y : _p1._y) + fromMinY;
	return Point(x, y, name);
}

std::ostream& operator<<(std::ostream& os, const Segment& s)
{
	return os << s._p1._name << s._p2._name;
}