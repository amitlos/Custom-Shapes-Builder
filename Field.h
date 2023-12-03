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
		static char defName;
		unsigned int _x;
		unsigned int _y;
		char _name = defName++;
	};

	struct Line 
	{
		Point _p1;
		Point _p2;
		std::string _name = std::string(&_p1._name, 1) + std::string(&_p2._name, 1);
		bool isInf;
	};

	Field(const char* input) : _input(input), _lexer(new Lexer(input)), _error_list(new std::string("")) { parse(); }
	~Field() { delete _error_list; delete _lexer; }
	bool hasError();
	const std::string& getLexErrors() { return _lexer->getError(); }
	const std::string& getSyntErrors() { return *_error_list; }
	void drawAll();
	const std::vector<Point>& getPoints() { return _points; }

private:
	const char* _input;
	std::vector<Point> _points;
	std::vector<Line> _lines;
	Lexer* _lexer;
	std::string* _error_list;
	

	void parse();
	bool parseDrPoints();
	bool parseName();
	bool parseDotArgs();
	void addPoint();
	bool parseDrLines();
	bool parseLineArgs();
	void addLine();
	bool parseConnect();
	bool parseMPoints();
	void getNextInstr();



};

std::ostream& operator<<(std::ostream&, const Field::Point&);

