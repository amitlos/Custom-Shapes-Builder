#pragma once
#include <SFML/Graphics.hpp>
#include "Point.h"
#include "Line.h"
#include "Segment.h"
#include "Triangle.h"


class Drawer
{
private:
	sf::RenderWindow& _window;
	unsigned int _width, _height;
public:
	Drawer(sf::RenderWindow& window) : _window(window), _width(window.getSize().x), _height(window.getSize().y) {}
	~Drawer() {}
	
	// drawing functions for single objects
	void drawPoint(const Point& p);

	void drawLine(const Line& line);

	void drawSegment(const Segment& s);

	void drawTriangle(const Triangle& t);

	// drawing functions for multiple objects
	template<class T>
	void drawPoints(const T&);

	template<class T>
	void drawLines(const T&);

	template<class T>
	void drawSegments(const T&);

	template<class T>
	void drawTriangles(const T&);
	
};

template<class T>
void Drawer::drawPoints(const T& points)
{
	for (const Point& p : points)
		drawPoint(p);
}

template<class T>
void Drawer::drawLines(const T& lines)
{
	for (const Line& l : lines)
		drawLine(l);
}

template<class T>
void Drawer::drawSegments(const T& segments)
{
	for (const Segment& s : segments)
		drawSegment(s);
}

template<class T>
void Drawer::drawTriangles(const T& triangles)
{
	for (const Triangle& t : triangles)
		drawTriangle(t);
}
