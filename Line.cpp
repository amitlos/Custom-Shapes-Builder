#include "Line.h"

double Line::getCoef() const 
{ 
	return (double)(_y2 - _y1) / (_x2 - _x1); 
}

double Line::getb() const 
{ 
	return (double)_y1 - getCoef() * _x1; 
}

std::ostream& operator<<(std::ostream& os, const Line& l)
{
	return os << l._name << "((" << l._x1 << ", " << l._y1 << "), (" << l._x2 << ", " << l._y2 << "))";
}