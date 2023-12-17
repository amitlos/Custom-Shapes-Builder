#pragma once
#include <iostream>
#include <queue>


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
		BUILD_RECT,
		BUILD_TRIANGLE, 
		BUILD_CIRCLE,
		BUILD_REGNGON,
		// symbols that will be used to name dots, lines
		NAME,
		// names of the figures
		SEGNAME, 
		RECTNAME,
		TRINAME,
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
	bool isEmpty() { return _nextT == Token::NULL_TERM; }
	bool hasError() { return _error_message != "" ; }
	const std::string& getError() { return _error_message; }

private:
	const char* _input;
	const char* _cursor;
	std::queue<char> _names;
	std::queue<unsigned int> _args;
	
	Token _nextT;

	std::string _error_message;

	// functions to help
	Token next();
	bool isLetter(char);
	bool isDigit(char);
	bool checkKW(const std::string& kword);    // checks whether the beginning of the input mathches a given key word
	Token getKeyWord();           // returns particular key word or unexpected token where at least two letters are detected 
	int getArg();				  // returns an integer number where the digit is detected
	Token getObjName();           // returns the corresponding token for the name of the object
	void ignoreSpaces();          // ignores all spaces  
	void errorCase();             // in the case of the lexical erorr it just escapes all that is before the next instruction (after the closest ';') 
};

std::ostream& operator<<(std::ostream& os, const Lexer::Token& kind);