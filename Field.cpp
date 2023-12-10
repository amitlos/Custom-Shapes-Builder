#include "Field.h"
#include <random>
#define TSpace Lexer::Token            // namespace of the class Token 

void Field::parse()
{
	// Erorr handling: The idea is: whenever we face the lexical or syntax error, we are getting straight to the getToken ';'(end of the current instruction)
	// to escape it, writing down the error message.
	// After that, we work as usual. 
	// If there isn`t an ending symbol, it stops on \0' terminal, we see this and tell user, 
	// that there is a syntax error - there is no end of the instruction. '
	bool notEnded = true;
	while (notEnded)
	{
		Lexer::Token t = _lexer->getToken();
		switch (t)
		{
		case TSpace::DRAW_POINTS:
			if(!parseDrPoints())
				getNextInstr();
			break;
		case TSpace::DRAW_LINES:
			if (!parseDrLines())
				getNextInstr();
			break;
		case TSpace::CONNECT:
			if (!parseConnect())
				getNextInstr();
			break;
		case TSpace::MARK_POINTS:
			if (!parseMPoints())
				getNextInstr();
			break;
		case TSpace::END:
			notEnded = false;
			break;
		case TSpace::UNEXPECTED:
			*_error_list += "Syntax error. Command is not found.\n";
			_error_token = new Lexer::Token(t);
			getNextInstr();
			break;
		case TSpace::NULL_TERM:
			*_error_list += "Syntax Error. End of the instructions wasn`t found. Please, check the rules and try again.\n";
			notEnded = false;
			break;
		default:
			*_error_list += "Syntax Error. Failed to read an instruction. Please, check the rules and try again.\n";
			_error_token = new Lexer::Token(t);
			getNextInstr();
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
				_var_args.push(x);
				_var_args.push(y);
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
				_var_args.push(x);
				_var_args.push(y);
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

void Field::addPoint()
{
	int x, y;
	if (_var_args.empty())
	{
		x = _lexer->nextArg();
		y = _lexer->nextArg();
	}
	else {
		x = _var_args.front();
		_var_args.pop();
		y = _var_args.front();
		_var_args.pop();
	}
	char name = _lexer->nextName();
	Point p(x, y, name);
	if (std::find(_points.begin(), _points.end(), p) == _points.end())
		_points.push_back(p);
	else
	{
		*_error_list += "Semantic error. Point with name \'"; *_error_list += p._name; *_error_list += "\' already exists.\n";
	}
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

	return true;

}
bool Field::parseDotArgs()
{
	int dot[] = { 10, 6, 7, 6, 9 }; // ( NUM , NUM )
	Lexer::Token t; 
	for (int i = 0; i < 5; i++) {
		t = _lexer->getToken();
		if (t != static_cast<TSpace>(dot[i]))
		{
			_error_token = new Lexer::Token(t);
			return false;
		}
	}
	return true;
}
bool Field::parseDrLines()
{
	if (_lexer->peek() != TSpace::SEMICOLON)
	{
		if (parseName())
		{
			_buffer = _lexer->nextName();
			if (_lexer->peek() == TSpace::COMA || _lexer->peek() == TSpace::SEMICOLON)
			{
				setRandLineArgs();
				addLine();
			}
			else if (parseLineArgs())
				addLine();
			else
				return false;
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
			_buffer = _lexer->nextName();
			if (_lexer->peek() == TSpace::COMA || _lexer->peek() == TSpace::SEMICOLON)
			{
				setRandLineArgs();
				addLine();
			}
			else if (parseLineArgs())
				addLine();
			else
				return false;
		}
		else
			return false;
	}

	_lexer->getToken();

	return true;
}

void Field::setRandLineArgs()
{
	_var_args.push(abs(rand() % 1000));
	_var_args.push(abs(rand() % 1000));
	_var_args.push(abs(rand() % 1000));
	_var_args.push(abs(rand() % 1000));
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

void Field::addLine()
{
	char name = _buffer;
	int coords[4];

	int i = 0;
	while(!_var_args.empty())
	{
		coords[i++] = _var_args.front();
		_var_args.pop();
	}

	while (i < 4)
		coords[i++] = _lexer->nextArg();

	_lines.push_back(Line(coords[0], coords[1], coords[2], coords[3], name));

	return;

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
	{
		char name = _lexer->nextName();
		auto it = std::find(_points.begin(), _points.end(), Point(0, 0, name));
		if (it == _points.end())
		{
			*_error_list += "Semantic Error. There is no such point ";
			*_error_list += std::string(1	, name);
			*_error_list += " in the scope.\n";
			_var_args.push(0);              // Default point is pushed - (0,0)
			_var_args.push(0);
		}
		else
		{
			_var_args.push((*it)._x);
			_var_args.push((*it)._y);
		}
	}
	else
	{
		_error_token = new Lexer::Token(t);
		return false;
	}

	return true;
		
}

bool Field::parseConnect()
{
	return true;
}
bool Field::parseMPoints()
{
	return true;
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