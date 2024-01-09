#include "Point.h"

//Constructor
Point::Point(int x, int y, char name) : _x(x), _y(y), _name(name) {}

bool Point::operator==(const Point& p) const
{
	return (_name == p._name);
}

std::ostream& operator<<(std::ostream& os, const Point& p)
{
	return os << p._name << '(' << p._x << ',' << p._y << ')';
}