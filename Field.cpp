#include "Field.h"
#include <SFML/Graphics.hpp>
#include <random>
#define TSpace Lexer::Token            // namespace of the class Token 

void Field::drawAll()
{
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Geometrical Interpreter");

	while (window.isOpen())
	{


		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);

		// Drawing points	
		for (Point p : _points)
			drawPoint(window, p);

		// Drawing lines
		for (Line l : _lines)
			drawLine(window, l);

		// Drawing segments
		for(Segment s : _segments)
			drawSegment(window, s);

		window.display();
	}
	return;
}

void Field::drawPoint(sf::RenderWindow& window, const Field::Point& p)
{
	sf::CircleShape circle(5.0f);
	circle.setFillColor(sf::Color::Black);
	circle.setPosition(p._x, p._y);
	window.draw(circle);

	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		// error...
	}
	sf::Text text;
	text.setFont(font);
	text.setString(std::string(1, p._name));
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);
	text.setPosition(p._x - 25, p._y - 25);
	window.draw(text);
	return;
}

void Field::drawLine(sf::RenderWindow& window, const Field::Line& line)
{
	// It can be done in a better way, but I don`t know how to do it.
	// The idea is: we have two points, and we need to draw a line between them. To do this, 
	// we calculate the coordinates of the points where the line crosses the borders of the window, and connect them.
	unsigned int x1, x2, y1, y2;
	double koef = line.getCoef();
	double b = line.getb();
	if (koef == 0)
	{
		x1 = 0;
		x2 = 1000;
		y1 = y2 = (unsigned int)b;
	}
	else if (koef == INFINITY)
	{
		y1 = 0;
		y2 = 1000;
		x1 = x2 = (unsigned int)b;
	}

	if (koef > 0) {
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

		if (koef * 1000 + b < 1000)
		{
			x2 = 1000;
			y2 = (unsigned int)(koef * 1000 + b);
		}
		else
		{
			y2 = 1000;
			x2 = (unsigned int)((1000 - b) / koef);
		}
	}
	else if (koef < 0)
	{
		if ((unsigned int)(-b / koef) < 1000)
		{
			y1 = 0;
			x1 = (unsigned int)(-b / koef);
		}
		else {
			x1 = 1000;
			y1 = (unsigned int)(koef * 1000 + b);
		}

		if (b < 1000)
		{
					x2 = 0;
			y2 = (unsigned int)b;
		}
		else
		{
			y2 = 1000;
			x2 = (unsigned int)((1000 - b) / koef);
		}

	}


	sf::VertexArray lines(sf::LinesStrip, 2);
	lines[0].position = sf::Vector2f(x1, y1);
	lines[0].color = sf::Color::Black;
	lines[1].position = sf::Vector2f(x2, y2);
	lines[1].color = sf::Color::Black;

	window.draw(lines);


	return;
}

void Field::drawSegment(sf::RenderWindow& window, const Field::Segment& s)
{
	sf::VertexArray lines(sf::LinesStrip, 2);
	lines[0].position = sf::Vector2f(s._p1._x, s._p1._y);
	lines[0].color = sf::Color::Black;
	lines[1].position = sf::Vector2f(s._p2._x, s._p2._y);
	lines[1].color = sf::Color::Black;

	window.draw(lines);

	return;

}

void Field::parse()
{
	// Erorr handling: The idea is: whenever we face the lexical or syntax error, we are getting straight to the getToken ';'(end of the current instruction)
	// to escape it, writing down the error message.
	// After that, we work as usual. 
	// If there isn`t an ending symbol, it stops on \0' terminal, we see this and tell user, 
	// that there is a syntax error - there is no end of the instruction. '
	bool isParsed = true;
	while (true)
	{
		Lexer::Token t = _lexer->getToken();
		switch (t)
		{
		case TSpace::DRAW_POINTS:
			isParsed = parseDrPoints();
			break;
		case TSpace::DRAW_LINES:
			isParsed = parseDrLines();
			break;
		case TSpace::CONNECT:
			isParsed = parseConnect();
			break;
		case TSpace::MARK_POINTS:
			isParsed = parseMPoints();
			break;
		case TSpace::BUILD_RECT:
			isParsed = parseBRect();
			break;
		case TSpace::BUILD_TRIANGLE:
			isParsed = parseBTriangle();
			break;
		case TSpace::BUILD_CIRCLE:
			isParsed = parseBCircle();
			break;
		case TSpace::BUILD_REGNGON:
			isParsed = parseBReg();
			break;
		case TSpace::END:
			return;
		case TSpace::UNEXPECTED:
			*_error_list += "Syntax error. Command is not found.\n";
			_error_token = new Lexer::Token(t);
			isParsed = false;
			break;
		case TSpace::NULL_TERM:
			*_error_list += "Syntax Error. End of the instructions wasn`t found. Please, check the rules and try again.\n";
			return;
		default:
			*_error_list += "Syntax Error. Failed to read an instruction. Please, check the rules and try again.\n";
			_error_token = new Lexer::Token(t);
			isParsed = false;
		}
		if (!isParsed)
		{
			getNextInstr();
			clearArgs();
		}
	}
}

bool Field::parseDrPoints()
{
	Lexer::Token t;
	if (_lexer->peek() != TSpace::SEMICOLON)
	{
		if (parseName())
		{
			if (_lexer->peek() == TSpace::COMA || _lexer->peek() == TSpace::SEMICOLON)
			{
				unsigned int x = abs(rand() % 1000);
				unsigned int y = abs(rand() % 1000);
				_cstr_nums.push(x);
				_cstr_nums.push(y);
				addPoint();
			}
			else if (parseDotArgs())
			{
				addPoint();
			}
			else
				return false;
		}
		else
			return false;
	}
	while(_lexer->peek() != Lexer::Token::SEMICOLON)
	{
		t = _lexer->getToken();
		if (t != TSpace::COMA)
		{
			_error_token = new Lexer::Token(t);
			return false;
		}
		if (parseName())
		{
			if (_lexer->peek() == TSpace::COMA || _lexer->peek() == TSpace::SEMICOLON)
			{
				unsigned int x = abs(rand() % 1000);
				unsigned int y = abs(rand() % 1000);
				_cstr_nums.push(x);
				_cstr_nums.push(y);
				addPoint();
			}
			else if (parseDotArgs())
			{
				addPoint();
			}
			else
				return false;
		}
		else
			return false;
	}

	_lexer->getToken();

	return true;
}

bool Field::parseName()
{
	Lexer::Token t;
	t = _lexer->getToken();
	if (t != TSpace::NAME)
	{
		_error_token = new Lexer::Token(t);
		return false;
	}

	_cstr_names.push(_lexer->nextName());

	return true;

}

bool Field::parseDotArgs()
{
	Lexer::Token dot[] = { TSpace::LEFT_BRACKET, TSpace::NUM, TSpace::COMA, TSpace::NUM, TSpace::RIGHT_BRACKET}; // ( x , y )
	Lexer::Token t; 
	for (int i = 0; i < 5; i++) {
		t = _lexer->getToken();
		if (t != dot[i])
		{
			_error_token = new Lexer::Token(t);
			return false;
		}
		if(t == TSpace::NUM)
			_cstr_nums.push(_lexer->nextArg());
	}

	return true;
}

bool Field::parseDrLines()
{
	if (_lexer->peek() != TSpace::SEMICOLON)
	{
		if (parseName())
		{
			if (_lexer->peek() == TSpace::COMA || _lexer->peek() == TSpace::SEMICOLON)
				setRandLineArgs();
			else 
			{
				if (!parseLineArgs())
					return false;
			}
			addLine();
		}
		else
			return false;
	}

	while (_lexer->peek() != TSpace::SEMICOLON)
	{
		Lexer::Token t = _lexer->getToken();
		if (t != TSpace::COMA)
		{
			_error_token = new Lexer::Token(t);
			return false;
		}
		if (parseName())
		{
			if (_lexer->peek() == TSpace::COMA || _lexer->peek() == TSpace::SEMICOLON)
				setRandLineArgs();
			else
			{
				if (!parseLineArgs())
					return false;
			}
			addLine();
		}
		else
			return false;
	}

	_lexer->getToken();

	return true;
}

void Field::setRandLineArgs()
{
	_cstr_nums.push(abs(rand() % 1000));
	_cstr_nums.push(abs(rand() % 1000));
	_cstr_nums.push(abs(rand() % 1000));
	_cstr_nums.push(abs(rand() % 1000));

	return;
}

bool Field::parseLineArgs()
{
	Lexer::Token t;

	if ((t = _lexer->getToken()) != TSpace::LEFT_BRACKET || !parseLineArg() ||
		(t = _lexer->getToken()) != TSpace::COMA		 || !parseLineArg() ||
		(t = _lexer->getToken()) != TSpace::RIGHT_BRACKET)	
	{
		if (!_error_token)
			_error_token = new Lexer::Token(t);
		return false;
	}

	return true;
}

bool Field::parseLineArg()
{
	Lexer::Token t;

	if (_lexer->peek() == TSpace::LEFT_BRACKET)
	{
		if (!parseDotArgs())
			return false;
	}
	else if ((t = _lexer->getToken()) == TSpace::NAME)
		 _cstr_names.push(_lexer->nextName());
	else
	{
		_error_token = new Lexer::Token(t);
		return false;
	}

	return true;
		
}

bool Field::parseConnect()
{
	TSpace t;
	if (!parseName())
		return false;
	if ((t = _lexer->getToken()) != TSpace::COMA)
	{
		_error_token = new Lexer::Token(t);
		return false;
	}
	if (!parseName())
		return false;

	t = _lexer->getToken();

	if (t != TSpace::SEMICOLON)
	{
		_error_token = new Lexer::Token(t);
		return false;
	}

	addSegment();

	return true;
}

bool Field::parseMPoints()
{
	Lexer::Token t;

	if (!parseName() || !parseDotArgs())
		return false;

	t = _lexer->getToken();
	if (t != TSpace::ON)
	{
		_error_token = new Lexer::Token(t);
		return false;
	}

	if(!parseSegment())
		return false;

	t = _lexer->getToken();
	if (t != TSpace::SEMICOLON)
	{
		_error_token = new Lexer::Token(t);
		return false;
	}

	addPointOnSeg();

	return true;
}

bool Field::parseSegment()
{
	TSpace t; 
	t = _lexer->getToken();
	if (t != TSpace::SEGNAME)
	{
		_error_token = new Lexer::Token(t);
		return false;
	}
	_cstr_names.push(_lexer->nextName());
	_cstr_names.push(_lexer->nextName());
	return true;
}

bool Field::parseBRect()
{
	return true;
}

bool Field::parseBTriangle()
{
	return true;
}

bool Field::parseBCircle()
{
	return true;
}

bool Field::parseBReg()
{
	return true;
}

void Field::addPoint()
{
	int x, y;
	char name;
	x = _cstr_nums.front();
	_cstr_nums.pop();
	y = _cstr_nums.front();
	_cstr_nums.pop();
	name = _cstr_names.front();
	_cstr_names.pop();
	Point p(x, y, name);
	if (std::find(_points.begin(), _points.end(), p) == _points.end())
		_points.push_back(p);
	else
	{
		*_error_list += "Semantic error. Point with name \'"; *_error_list += p._name; *_error_list += "\' already exists.\n";
	}
}

void Field::addLine()
{
	char name = _cstr_names.front();
	_cstr_names.pop();

	while (!_cstr_names.empty())
		getCoords();

	int x1 = _cstr_nums.front();
	_cstr_nums.pop();
	int y1 = _cstr_nums.front();
	_cstr_nums.pop();
	int x2 = _cstr_nums.front();
	_cstr_nums.pop();
	int y2 = _cstr_nums.front();
	_cstr_nums.pop();

	_lines.push_back(Line(x1, y1, x2, y2, name));

	return;

}

void Field::addSegment()
{
	char name1 = _cstr_names.front();
	_cstr_names.pop();
	char name2 = _cstr_names.front();
	_cstr_names.pop();

	Point p1(0, 0, name1);
	Point p2(0, 0, name2);

	auto it1 = std::find(_points.begin(), _points.end(), p1);
	auto it2 = std::find(_points.begin(), _points.end(), p2);

	if (it1 == _points.end() || it2 == _points.end())
	{
		*_error_list += "Semantic Error. There is no such point ";
		return;
	}
	
	_segments.push_back(Segment(*it1, *it2));

	return;
}

void Field::addPointOnSeg()
{
	char name = _cstr_names.front();
	_cstr_names.pop();
	unsigned int p1 = _cstr_nums.front();
	_cstr_nums.pop();
	unsigned int p2 = _cstr_nums.front();
	_cstr_nums.pop();

	char name1 = _cstr_names.front();
	_cstr_names.pop();
	char name2 = _cstr_names.front();
	_cstr_names.pop();
	Point pt1(0, 0, name1);
	Point pt2(0, 0, name2);

	Segment s(pt1, pt2);

	auto it = std::find(_segments.begin(), _segments.end(), s);

	if (it == _segments.end())
	{
		*_error_list += "Semantic Error. There is no such segment ";
		return;
	}

	_points.push_back((*it).getPointOn(name, p1, p2));

	return;
}

void Field::addRect()
{

}

void Field::addTriangle()
{

}

void Field::addCircle()
{

}

void Field::addReg()
{

}

void Field::clearArgs()
{
	_cstr_names = std::queue<char>();
	_cstr_nums = std::queue<unsigned int>();
	return;
}

void Field::getCoords()
{	
	char name = _cstr_names.front();
	_cstr_names.pop();
	auto it = std::find(_points.begin(), _points.end(), Point(0, 0, name));
	if (it == _points.end())
	{
		*_error_list += "Semantic Error. There is no such point ";
		*_error_list += std::string(1, name);
		*_error_list += " in the scope.\n";
		_cstr_nums.push(0);              // Default point is pushed - (0,0)
		_cstr_nums.push(0);
	}
	else
	{
		_cstr_nums.push((*it)._x);
		_cstr_nums.push((*it)._y);
	}

	return;
}

void Field::getNextInstr()
{
	Lexer::Token temp; if (_error_token) temp = *_error_token; else temp = _lexer->getToken();
	if (_error_token) delete _error_token;
	_error_token = nullptr;
	while (temp != TSpace::SEMICOLON && temp != TSpace::NULL_TERM)
	{
		if (temp == TSpace::NAME) _lexer->nextName();
		if (temp == TSpace::NUM) _lexer->nextArg();
		temp = _lexer->getToken();
	}
	return;
}

bool Field::hasError() { return *_error_list != ""; }

std::ostream& operator<<(std::ostream& os, const Field::Point& p)
{
	return os << p._name << '(' << p._x << ',' << p._y << ')';
}

std::ostream& operator<<(std::ostream& os, const Field::Line& l)
{
	return os << l._name << "((" << l._x1 << ", " << l._y1 << "), (" << l._x2 << ", " << l._y2 << "))";
}

std::ostream& operator<<(std::ostream& os, const Field::Segment& s)
{
	return os << s._p1._name << s._p2._name;
}