#include "Field.h"

// Default constructor
Field::Triangle::Triangle() {}

// Constructor that takes all neccesary arguments to create a triangle in the field
Field::Triangle::Triangle(int x, int y, double l1, double l2, double angle, const char* names)
{
	_a._x = x;
	_a._y = y;
	_a._name = names[0];
	_b._x = x + (unsigned int)(l1 * cos(angle));	
	_b._y = y - (unsigned int)(l1 * sin(angle));
	_b._name = names[1];
	_c._x = _a._x + l2;
	_c._y = _a._y;
	_c._name = names[2];
}