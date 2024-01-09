#pragma once
#include "Point.h"
#include <iostream>

struct Triangle
{
	Point _a, _b, _c;

	Triangle();

	Triangle(int x, int y, double l1, double l2, double angle, const char* names);
};

std::ostream& operator<<(std::ostream&, const Triangle&);