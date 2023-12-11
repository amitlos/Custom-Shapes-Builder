#pragma once
/*
* File: Field.h
* Author: Solovei Tymofii
* Created on 2, December 2023 
* 
* Description:
* This class is responsible for parsing the input and storing the data about geometrical objects.
* Also it draws all the objects using SFML library via drawAll() methods.
*/


#include <iostream>
#include <vector>
#include "Lexer.h"
#include <SFML/Graphics.hpp>

class Field
{
public:
	//after debug, make these structures private!
	// structure to store points
	struct Point
	{
		Point(unsigned int x = 0, unsigned int y = 0, char name = 'W') : _x(x), _y(y), _name(name) {}
		inline bool operator== (const Point& p) const { return _name == p._name; }
		unsigned int _x;
		unsigned int _y;
		char _name;
	};

	// structure to store lines
	struct Line 
	{
		double getCoef() const { return (double)(_y2 - _y1) / (_x2 - _x1); };
		double getb() const { return (double)_y1 - getCoef() * _x1; };

		Line(int x1, int y1, int x2, int y2, char name = 'w', bool is_inf = true) : _x1(x1), _y1(y1), _x2(x2), _y2(y2), _name(name) {}
		int _x1;
		int _y1;
		int _x2;
		int _y2;
		char _name;

	};

	// structure to store segments
	struct Segment
	{
		// fields to store the points of the segment
		Point _p1, _p2;

		// constructs a segment using two points
		Segment(const Point& p1, const Point& p2) : _p1(p1), _p2(p2) {}

		inline bool operator== (const Segment& s) const { return (_p1 == s._p1 && _p2 == s._p2) || (_p1 == s._p2 && _p2 == s._p1); }

		// function to get the length of the segment
		double getLength() const;

		// function to get the coordinates of the point on the segment using the proportion
		Point getPointOn(char, unsigned int, unsigned int) const;	
	};

	// constructs a field using the input string
	Field(const char* input) : _input(input), _lexer(new Lexer(input)), _error_list(new std::string("")), _error_token(nullptr) { parse(); }
	~Field() { delete _error_list; delete _lexer; }

	// function to check whether there is an error
	bool hasError();

	// function to get the list of the lexical errors
	const std::string& getLexErrors() { return _lexer->getError(); }

	// function to get the list of the syntax errors
	const std::string& getSyntErrors() { return *_error_list; }

	// function to draw all the objects
	void drawAll();

	// function to get the points
	inline const std::vector<Point>& getPoints() { return _points; }

	// function to get the lines
	inline const std::vector<Line>& getLines() { return _lines; }

	// function to get the segments
	inline const std::vector<Segment>& getSegments() { return _segments; }

private:
	// Data
	const char* _input;                           // input string
	std::vector<Point> _points;                   // vector to store points
	std::vector<Line> _lines;                     // vector to store lines
	std::vector<Segment> _segments;               // vector to store segments

	// Lexer
	Lexer* _lexer;                                // lexer to get a token stream based on the input string

	// Errors data
	std::string* _error_list;					  // list of the syntax errors
	Lexer::Token* _error_token;					  // token that caused the last error

	// Storing arguments for the object construction
	std::queue<unsigned int> _cstr_nums;          // queue with numbers needed to construct particular object
	std::queue<char> _cstr_names;                 // queue with names needed to construct particular object
	                                            
	             
	// function that parses a list of instructions
	void parse();

	// parsing of particular instructions
	// parsing DRAW_POINTS instruction
	bool parseDrPoints();
	bool parseName();
	bool parseDotArgs();

	// parsing DRAW_LINES instruction
	bool parseDrLines();
	bool parseLineArgs();
	bool parseLineArg();
	void setRandLineArgs();

	//parsing CONNECT instruction
	bool parseConnect();

	// parsing MARK_POINTS instruction
	bool parseMPoints();
	bool parseSegment();

	// parsing BUILD_RECT instruction
	bool parseBRect();

	// parsing BUILD_TRIANGLE instruction
	bool parseBTriangle();

	// parsing BUILD_CIRCLE instruction
	bool parseBCircle();

	// parsing BUILD_REGNGON instruction
	bool parseBReg();

	//object building functions
	void addPoint();
	void addLine();
	void addSegment();
	void addPointOnSeg();
	void addRect();
	void addTriangle();
	void addCircle();
	void addReg();

	void drawPoint(sf::RenderWindow& window, const Point& p);
	void drawLine(sf::RenderWindow& window, const Line& line);
	void drawSegment(sf::RenderWindow& window, const Segment& s);

	// function to get next instruction after getting syntax error
	void getNextInstr();

	// clears the queues with arguments
	void clearArgs();

	// function that takes a name from the _cstr_names, finds the point with this name in the _points, 
	// and puts its coordinates to _cstr_nums 
	void getCoords(); 
		
};

// operators to print the objects
std::ostream& operator<<(std::ostream&, const Field::Point&);
std::ostream& operator<<(std::ostream&, const Field::Line&);
std::ostream& operator<<(std::ostream&, const Field::Segment&);
