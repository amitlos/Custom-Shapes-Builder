#include "Field.h"


char Field::Point::defName = ('Z' - 'A') / 2;

void Field::parse()
{
	// Erorr handling: The idea is: whenever we face the lexical or syntax error, we are getting straight to the getToken ';'(end of the current instruction)
	// to escape it, writing down the error message.
	// After that, we work as usual. 
	// If there isn`t an ending symbol, it stops on '\0' terminal, we see this and tell user, 
	// that there is a syntax error - there is no end of the instruction. 
	bool notEnded = true;
	while (notEnded)
	{
		switch (_lexer->getToken())
		{
		case Lexer::Token::DRAW_POINTS:
			if(!parseDrPoints())
				getNextInstr();
			break;
		case Lexer::Token::DRAW_LINES:
			if (!parseDrLines())
				getNextInstr();
			break;
		case Lexer::Token::CONNECT:
			if (!parseConnect())
				getNextInstr();
			break;
		case Lexer::Token::MARK_POINTS:
			if (!parseMPoints())
				getNextInstr();
			break;
		case Lexer::Token::END:
			notEnded = false;
			break;
		case Lexer::Token::UNEXPECTED:
			*_error_list += "Syntax error. Command is not found.\n";
			getNextInstr();
			break;
		case Lexer::Token::NULL_TERM:
			*_error_list += "Syntax Error. End of the instructions wasn`t found. Please, check the rules and try again.\n";
			notEnded = false;
			break;
		default:
			*_error_list += "Syntax Error. Failed to read an instruction. Please, check the rules and try again.\n";
			getNextInstr();
		}
	}
}

bool Field::parseDrPoints()
{
	if (_lexer->peek() != Lexer::Token::SEMICOLON)
	{
		if (parseName() && parseDotArgs())
			addPoint();
		else
			return false;
	}
	while(_lexer->peek() != Lexer::Token::SEMICOLON)
	{
		if (_lexer->getToken() != Lexer::Token::COMA)
			return false;
		if (parseName() && parseDotArgs())
			addPoint();
		else
			return false;
	}

	_lexer->getToken();

	return true;
}

void Field::addPoint()
{
	int x = _lexer->nextArg();
	int y = _lexer->nextArg();
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
	return (_lexer->getToken() == Lexer::Token::NAME);
}
bool Field::parseDotArgs()
{
	int dot[] = { 10, 6, 7, 6, 9 }; // ( NUM , NUM )
	for (int i = 0; i < 5; i++)
		if (_lexer->getToken() != static_cast<Lexer::Token>(dot[i]))
			return false;
	return true;
}
bool Field::parseDrLines()
{
	if (_lexer->peek() != Lexer::Token::SEMICOLON)
	{
		if (parseName() && parseLineArgs())
			addLine();
		else
			return false;
	}
	while (_lexer->peek() != Lexer::Token::SEMICOLON)
	{
		if (_lexer->getToken() != Lexer::Token::COMA)
			return false;
		if (parseName() && parseLineArgs())
			addLine();
		else
			return false;
	}

	_lexer->getToken();

	return true;
}
bool Field::parseLineArgs()
{
	Point a;
	Point b;
	std::string name;

	if (_lexer->getToken() != Lexer::Token::LEFT_BRACKET)
		return false;
	if (_lexer->peek() == Lexer::Token::LEFT_BRACKET)
	{
		if (!parseDotArgs())
			return false;
	}
	else if (_lexer->getToken() != Lexer::Token::NAME)
		return false;
	
	if (_lexer->getToken() != Lexer::Token::COMA)
		return false;

	if (_lexer->peek() == Lexer::Token::LEFT_BRACKET)
	{
		if (!parseDotArgs())
			return false;
	}
	else if (_lexer->getToken() != Lexer::Token::NAME)
		return false;

	if (_lexer->getToken() != Lexer::Token::RIGHT_BRACKET)
		return false;

	return true;
}

void Field::addLine()
{

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
	Lexer::Token temp = _lexer->getToken();
	while(temp != Lexer::Token::SEMICOLON && temp != Lexer::Token::NULL_TERM)
		temp = _lexer->getToken();
	return;
}

bool Field::hasError() { return *_error_list != ""; }

std::ostream& operator<<(std::ostream& os, const Field::Point& p)
{
	return os << p._name << '(' << p._x << ',' << p._y << ')';
}