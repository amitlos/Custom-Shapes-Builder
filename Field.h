#pragma once
#include <iostream>
#include <vector>
#include "Lexer.h"

class Field
{
public:
	struct Point
	{
		Point(unsigned int x = 0, unsigned int y = 0, char name = 'W') : _x(x), _y(y), _name(name) {}
		inline bool operator== (const Point& p) { return _name == p._name; }
		unsigned int _x;
		unsigned int _y;
		char _name;
	};

	struct Line 
	{
		Line(int x1, int y1, int x2, int y2, char name = 'w', bool is_inf = true) : _x1(x1), _y1(y1), _x2(x2), _y2(y2), _name(name), _is_inf(is_inf) {}
		int _x1;
		int _y1;
		int _x2;
		int _y2;
		char _name;
		bool _is_inf;
	};

	Field(const char* input) : _input(input), _lexer(new Lexer(input)), _error_list(new std::string("")), _error_token(nullptr) { parse(); }
	~Field() { delete _error_list; delete _lexer; }
	bool hasError();
	const std::string& getLexErrors() { return _lexer->getError(); }
	const std::string& getSyntErrors() { return *_error_list; }
	void drawAll();
	inline const std::vector<Point>& getPoints() { return _points; }
	inline const std::vector<Line>& getLines() { return _lines; }

private:
	const char* _input;
	std::vector<Point> _points;
	std::vector<Line> _lines;
	Lexer* _lexer;
	std::string* _error_list;
	char _buffer;                                 // buffer to store the name of the line
	std::queue<unsigned int> _var_args;           // queue with arguments stored in variables
	                                              //(for e. coordinates of points A(ax,ay) B(bx,by) in definition of line LINE(A,B) will be placed there)
	Lexer::Token* _error_token;

	void parse();
	bool parseDrPoints();
	bool parseName();
	bool parseDotArgs();
	void addPoint();
	bool parseDrLines();
	bool parseLineArgs();
	bool parseLineArg();
	void addLine();
	bool parseConnect();
	bool parseMPoints();
	void getNextInstr();



};

std::ostream& operator<<(std::ostream&, const Field::Point&);
std::ostream& operator<<(std::ostream&, const Field::Line&);

