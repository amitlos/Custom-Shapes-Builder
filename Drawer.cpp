#include "Drawer.h"

void Drawer::drawPoint(const Point& p)
{
	sf::CircleShape circle(6.0f);
	circle.setFillColor(sf::Color::Black);
	circle.setPosition(p._x - 3.0f, p._y - 3.0f);
	_window.draw(circle);

	sf::Font font;
	if (!font.loadFromFile("./resources/arial.ttf"))
	{
		return;
		// error...
	}
	sf::Text text;
	text.setFont(font);
	text.setString(std::string(1, p._name));
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);
	text.setPosition(p._x - 25, p._y - 25);
	_window.draw(text);
	return;
}

void Drawer::drawLine(const Line& line)
{
	// The idea is: we have two points, and we need to draw a line between them. To do this, 
// we calculate the coordinates of the points where the line crosses the borders of the window, and connect them.
	unsigned int x1, x2, y1, y2;
	double koef = line.getCoef();
	double b = line.getb();

	if (koef == 0)
	{
		x1 = 0;
		x2 = _width;
		y1 = y2 = (unsigned int)b;
	}
	else if (koef == INFINITY)
	{
		y1 = 0;
		y2 = _height;
		x1 = x2 = line._x1;
	}
	else if (koef > 0)
	{
		if (b >= 0)
		{
			x1 = 0;
			y1 = (unsigned int)b;
		}
		else
		{
			y1 = 0;
			x1 = (unsigned int)(-b / koef);
		}

		if (koef * _width + b < _width)
		{
			x2 = _width;
			y2 = (unsigned int)(koef * x2 + b);
		}
		else
		{
			y2 = _width;
			x2 = (unsigned int)((y2 - b) / koef);
		}
	}
	else if (koef < 0)
	{
		if ((unsigned int)(-b / koef) < _width)
		{
			y1 = 0;
			x1 = (unsigned int)(-b / koef);
		}
		else {
			x1 = _width;
			y1 = (unsigned int)(koef * x1 + b);
		}

		if (b < _width)
		{
			x2 = 0;
			y2 = (unsigned int)b;
		}
		else
		{
			y2 = _width;
			x2 = (unsigned int)((y2 - b) / koef);
		}
	}


	sf::VertexArray lines(sf::LinesStrip, 2);
	lines[0].position = sf::Vector2f(x1, y1);
	lines[0].color = sf::Color::Black;
	lines[1].position = sf::Vector2f(x2, y2);
	lines[1].color = sf::Color::Black;

	_window.draw(lines);


	return;
}

void Drawer::drawSegment(const Segment& s)
{
	sf::VertexArray lines(sf::LinesStrip, 2);
	lines[0].position = sf::Vector2f(s._p1._x, s._p1._y);
	lines[0].color = sf::Color::Black;
	lines[1].position = sf::Vector2f(s._p2._x, s._p2._y);
	lines[1].color = sf::Color::Black;

	_window.draw(lines);

	return;
}

void Drawer::drawTriangle(const Triangle& t)
{
	drawSegment(Segment(t._a, t._b));
	drawSegment(Segment(t._a, t._c));
	drawSegment(Segment(t._c, t._b));

	drawPoint(t._a);
	drawPoint(t._b);
	drawPoint(t._c);

	return;
}