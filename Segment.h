#pragma once
#include "Point.h"
// structure to store segments
struct Segment
{
	// fields to store the points of the segment
	Point _p1, _p2;

	//Default constructor
	Segment() {}

	// constructs a segment using two points
	Segment(const Point& p1, const Point& p2) : _p1(p1), _p2(p2) {}

	inline bool operator== (const Segment& s) const { return (_p1 == s._p1 && _p2 == s._p2) || (_p1 == s._p2 && _p2 == s._p1); }

	// function to make the point on the segment using the proportion
	Point getPointOn(char, unsigned int, unsigned int) const;
};

std::ostream& operator<<(std::ostream& os, const Segment& s);