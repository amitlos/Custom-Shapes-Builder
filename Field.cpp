#include "Field.h"
#include <SFML/Graphics.hpp>
#include <random>
#define TSpace Lexer::Token            // namespace of the values of Token enumeration 

// Constants
#define pixpercm 38
#define PI 3.14159

// Default values for triangle 
#define dl1 3          // length of the first side 
#define dl2 5		   // length of the second side 
#define dangle 45      // angle between sides 

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
	setRandomPosition();
	_cstr_nums.push(abs(rand() % 1000));
	_cstr_nums.push(abs(rand() % 1000));

	return;
}

void Field::setRandomPosition()
{
	_cstr_nums.push(abs(rand() % 800) + 100);                   
	_cstr_nums.push(abs(rand() % 800) + 100);
}

void Field::setDefTriArgs()
{

	setRandomPosition();
	_cstr_nums.push(dangle);						               
	_cstr_nums.push(dl1);                   
	_cstr_nums.push(dl2);

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

	if(!parseSegName())
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

bool Field::parseSegName()
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


bool Field::parseTriName()
{
	TSpace t;
	t = _lexer->getToken();
	if (t != TSpace::TRINAME)
	{
		_error_token = new Lexer::Token(t);
		return false;
	}
	_cstr_names.push(_lexer->nextName());
	_cstr_names.push(_lexer->nextName());
	_cstr_names.push(_lexer->nextName());
	return true;
}

bool Field::parseBTriangle()
{
	Lexer::Token t;

	if (!parseTriName())
		return false;

	if (_lexer->peek() == TSpace::SEMICOLON)
	{
		setDefTriArgs();
	}
	else if (!parseTriArgs())
		return false;

	t = _lexer->getToken();
	if (t != TSpace::SEMICOLON)
	{
		_error_token = new Lexer::Token(t);
		return false;
	}

	addTriangle();

	return true;
}

bool Field::parseTriArgs()
{

	setRandomPosition();	

	Lexer::Token tri_parameters[] = { TSpace::LEFT_BRACKET, TSpace::NUM, TSpace::COMA, TSpace::NUM, TSpace::COMA, TSpace::NUM, TSpace::RIGHT_BRACKET };

	Lexer::Token t;
	for (int i = 0; i < 7; i++)
	{
		t = _lexer->getToken();
		if (t != tri_parameters[i])
		{
			_error_token = new Lexer::Token(t);
			return false;
		}
		if (t == TSpace::NUM)
			_cstr_nums.push(_lexer->nextArg());
	}

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
	//taking names
	char names[3];

	for (int i = 0; i < 3; i++)
	{
		names[i] = _cstr_names.front();
		_cstr_names.pop();
	}

	// taking position
	unsigned int x = _cstr_nums.front();
	_cstr_nums.pop();
	unsigned int y = _cstr_nums.front();
	_cstr_nums.pop();

	//taking and converting values to correct units (length from cm to pixels, angle from degrees to radians)
	double radians = (_cstr_nums.front() / 180.0) * PI;
	_cstr_nums.pop();

	unsigned int l1 = _cstr_nums.front() * pixpercm;
	_cstr_nums.pop();
	unsigned int l2 = _cstr_nums.front() * pixpercm;
	_cstr_nums.pop();

	_triangles.push_back(Triangle(x, y, l1, l2, radians, names));

	return;
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
		switch (temp)
		{
		case TSpace::NAME:
			_lexer->nextName();
			break;
		case TSpace::NUM:
			_lexer->nextArg();
			break;
		case TSpace::SEGNAME:
			_lexer->nextName();
			_lexer->nextName();
			break;
		case TSpace::TRINAME:
			for (int i = 0; i < 3; i++)
				_lexer->nextName();
			break;
		case TSpace::RECTNAME:
			for(int i = 0; i < 4; i++)
				_lexer->nextName();
		} 
		
		temp = _lexer->getToken();
	}
	return;
}

bool Field::hasError() { return *_error_list != ""; }