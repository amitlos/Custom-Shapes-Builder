#pragma once
#include <iostream>
#include <queue>
#include "Field.h"


class Lexer
{
public:
	enum class Token
	{
		//instructions 
		DRAW_POINTS,
		DRAW_LINES,
		CONNECT,
		MARK_POINTS,
		ON,
		// symbols that will be used to name dots, lines
		NAME,
		// just some integer number
		NUM,
		// punctuation
		COMA,
		SEMICOLON, 
		RIGHT_BRACKET, 
		LEFT_BRACKET,
		TWODOTS,
		END,
		// in the case of error 
		UNEXPECTED,
		NULL_TERM
	};
	Lexer(const char* input) : _input(input), _cursor(_input) { _nextT = next(); }
	~Lexer() {}
	Token getToken() { Token temp = _nextT; _nextT = next(); return temp; }
	const Token& peek() { return _nextT; }
	char nextName();
	unsigned int nextArg();
	void toArg(const std::vector<Field::Point>&);
	bool isEmpty() { return _nextT == Token::NULL_TERM; }
	bool hasError() { return error_message != "" ; }
	const std::string& getError() { return error_message; }

private:
	const char* _input;
	const char* _cursor;
	std::queue<char> names;
	std::deque<unsigned int> args;
	
	Token _nextT;

	std::string error_message;

	// functions to help
	Token next();
	bool isLetter(char);
	bool isDigit(char);
	bool checkKW(const std::string& kword);    // checks whether the beginning of the input mathches a given key word
	Token getKeyWord();           // returns particular key word or unexpected token where at least two letters are detected 
	int getArg();				  // returns an integer number where the digit is detected
	void ignoreSpaces();          // ignores all spaces  
	void errorCase();             // in the case of the lexical erorr it just escapes all that is before the next instruction (after the closest ';') 
};

std::ostream& operator<<(std::ostream& os, const Lexer::Token& kind);