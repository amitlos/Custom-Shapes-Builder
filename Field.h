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
#include "Point.h"
#include "Line.h"
#include "Segment.h"
#include "Triangle.h"
#include <SFML/Graphics.hpp>

//#define NDEBUG

class Field
{
public:

	// constructs a field using the input string
	Field(const char* input) : _input(input), _lexer(new Lexer(input)), _error_list(new std::string("")), _error_token(nullptr) { parse(); }

	//Destructor
	~Field() { delete _error_list; delete _lexer; }

	// function to check whether there is an error
	bool hasError();

	// function to get the list of the lexical errors
	const std::string& getLexErrors() { return _lexer->getError(); }

	// function to get the list of the syntax errors
	const std::string& getSyntErrors() { return *_error_list; }

	// function to get the points
	inline const std::vector<Point>& getPoints() { return _points; }

	// function to get the lines
	inline const std::vector<Line>& getLines() { return _lines; }

	// function to get the segments
	inline const std::vector<Segment>& getSegments() { return _segments; }

	// function to get the triangles
	inline const std::vector<Triangle>& getTriangles() { return _triangles; }

private:
	// Data
	const char* _input;                           // input string
	std::vector<Point> _points;                   // vector to store points
	std::vector<Line> _lines;                     // vector to store lines
	std::vector<Segment> _segments;               // vector to store segments
	std::vector<Triangle> _triangles;

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
	bool parseSegName();

	// parsing BUILD_RECT instruction
	bool parseBRect();

	// parsing BUILD_TRIANGLE instruction
	bool parseBTriangle();
	bool parseTriName();
	bool parseTriArgs();
	void setDefTriArgs();

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

	// function to get next instruction after getting syntax error
	void getNextInstr();

	// clears the queues with arguments
	void clearArgs();

	// function that takes a name from the _cstr_names, finds the point with this name in the _points, 
	// and puts its coordinates to _cstr_nums 
	void getCoords(); 

	void setRandomPosition();
	
};

	