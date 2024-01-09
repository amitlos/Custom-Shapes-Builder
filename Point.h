#pragma once
#include <iostream>
struct Point
{
	Point(int x = 0, int y = 0, char name = 'R');
	bool operator== (const Point& p) const;
	int _x;
	int _y;
	char _name;
};

std::ostream& operator<<(std::ostream&, const Point&);