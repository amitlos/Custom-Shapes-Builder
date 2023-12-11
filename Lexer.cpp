#include "Lexer.h"

Lexer::Token Lexer::next()
{
	
	ignoreSpaces();
	if (*_cursor == '\0')
	{
		return Token::NULL_TERM;	
	}
	if (isLetter(*_cursor) && isLetter(*(_cursor + 1)))
	{
		Token kw = getKeyWord();
		if (kw == Token::UNEXPECTED) {
			//_error_message = "Lexical error. One of the key word wasn`t recognised correctly. Please, check the rules, and try again.";
			_names.push(*_cursor);
			_cursor++;
			return Token::NAME;
		}
		return kw;
	}
	else if (isLetter(*_cursor))
	{
		char name = *_cursor;
		_names.push(name);
		_cursor++;
		return Token::NAME;
	}
	else if (isDigit(*_cursor))
	{
		int arg = getArg();
		_args.push(arg);
		return Token::NUM;
	}


	switch (*_cursor)
	{
	case ',':
		_cursor++;
		return Token::COMA;
	case '(':
		_cursor++;
		return Token::LEFT_BRACKET;
	case ')':
		_cursor++;
		return Token::RIGHT_BRACKET;
	case ':':
		_cursor++;
		return Token::TWODOTS;
	case ';':
		_cursor++;
		return Token::SEMICOLON;
	case '#':
		_cursor++;
		return Token::END;
	default:
		errorCase();
		_error_message = "Lexical error. Some invalid symbol was found. Please, check the rules, and try again.";
		return Token::UNEXPECTED;
	}
}

char Lexer::nextName()
{
	if (!_names.empty())
	{
		char temp = _names.front();
		_names.pop();
		return temp;
	}
	else
		throw std::string("Attempt to get a name from empty queue!");            // change it, that is not normal
}
 
unsigned int Lexer::nextArg()
{
	if (!_args.empty())
	{
		int temp = _args.front();
		_args.pop();
		return temp;
	}
	else
		throw std::string("Attempt to get a number from empty queue!");
}

bool Lexer::isLetter(char elem)
{	
	return (elem >= 'A' && elem <= 'Z') || (elem >= 'a' && elem <= 'z');
}
Lexer::Token Lexer::getKeyWord()
{
	std::string KEY1("DRAW_POINTS");
	std::string KEY2("DRAW_LINES");
	std::string KEY3("CONNECT");
	std::string KEY4("MARK_POINTS");
	std::string KEY5("ON");
	std::string KEY6("BUILD_RECT");
	std::string KEY7("BUILD_TRIANGLE");
	std::string KEY8("BUILD_CIRCLE");
	std::string KEY9("BUILD_REGNGON");


	switch (*_cursor)
	{
	case 'M':
		return checkKW(KEY4) ? Token::MARK_POINTS : Token::UNEXPECTED;
	case 'O':
		return checkKW(KEY5) ? Token::ON : Token::UNEXPECTED;
	case 'D':
		return checkKW(KEY1) ? Token::DRAW_POINTS : (checkKW(KEY2) ? Token::DRAW_LINES : Token::UNEXPECTED);
	case 'C':
		return checkKW(KEY3) ? Token::CONNECT : Token::UNEXPECTED;
	case 'B':
		return checkKW(KEY6) ? Token::BUILD_RECT : (checkKW(KEY7) ? Token::BUILD_TRIANGLE : (checkKW(KEY8) ? Token::BUILD_CIRCLE : (checkKW(KEY9) ? Token::BUILD_REGNGON : Token::UNEXPECTED)));
	default:
		return Token::UNEXPECTED;
	}	
}

int Lexer::getArg()
{
	int res = (*_cursor) - '0';
	_cursor++;
	while (isDigit(*_cursor))
	{
		res *= 10;
		res += (*_cursor) - '0';
		_cursor++;
	}

	return res;
}
bool Lexer::isDigit(char elem)
{
	return (elem >= '0') && (elem <= '9');
}

bool Lexer::checkKW(const std::string& kword)
{
	for (int i = 0;i < kword.size(); i++)
		if (*(_cursor + i) != kword.at(i))
		{

			return false;
		}
	_cursor += kword.size();
	return true;
}

void Lexer::ignoreSpaces()
{
	while (*_cursor == ' ' || *_cursor == '\n' || *_cursor == '\t' || *_cursor == '\v')
		_cursor++;
	return;
}

void Lexer::errorCase()
{
	
	while (*_cursor != ' ' && *_cursor != ';' && *_cursor != '\0')
		_cursor++;
	return;
}

std::ostream& operator<<(std::ostream& os, const Lexer::Token& kind) {
	static const char* const names[]{
			"DRAW_POINTS",      "DRAW_LINES",  "CONNECT",  "MARK_POINTS", "ON",
			"BUILD_RECT", "BUILD_TRIANGLE", "BUILD_CIRCLE", "BUILD_REGNGON",
			"NAME",   "NUM", "COMA",   "SEMICOLON",
			"RIGHT_BRACKET",       "LEFT_BRACKET",        "TWODOTS",      "END",   "UNEXPECTED",
			"NULL_TERM",
	};
	return os << names[static_cast<int>(kind)];
}
