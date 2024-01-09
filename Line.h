#pragma once
#include <iostream>
struct Line
{
	double getCoef() const;
	double getb() const;

	Line(int x1, int y1, int x2, int y2, char name = 'w', bool is_inf = true) : _x1(x1), _y1(y1), _x2(x2), _y2(y2), _name(name) {}
	int _x1;
	int _y1;
	int _x2;
	int _y2;
	char _name;

};

std::ostream& operator<<(std::ostream& os, const Line& l);