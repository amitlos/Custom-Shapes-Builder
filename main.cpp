// It is the attempt to create a kind of compiler for geometrical obeject;
// Examples of the program code:
//DRAW_POINTS A, B, C; DRAW_LINES a, b, c;# 
//DRAW_POINTS A, B, C; DRAW_LINES a(A, C), b, c;CONNECT A,B; CONNECT B,C; MARK_POINTS K(1,1) ON AB; MARK_POINTS L(3,1) ON AB; #
// BUILD_TRIANGLE PKL;  BUILD_TRIANGLE PKL; BUILD_TRIANGLE CAB(90, 4, 3); BUILD_TRIANGLE JHG(120,10, 3 ); BUILD_TRIANGLE ZXC(60, 5, 5); BUILD_TRIANGLE NMK(30, 10, 17 );# 
// DRAW_LINES a((100,0), (100, 100));#
// DRAW_LINES a((100,100), (200, 100));#
// DRAW_POINTS A, B, C(100, 100); DRAW_LINES a, b(A, (100, 200)), c(A,B); CONNECT B,C; MARK_POINTS K (1,1) ON BC; MARK_POINTS L(1,2) ON BC; 
// BUILD_TRIANGLE PKL; BUILD_TRIANGLE CAB(90, 4, 3); BUILD_TRIANGLE JHG(120,10, 3 ); BUILD_TRIANGLE ZXC(60, 5, 5); BUILD_TRIANGLE NMK(30, 10, 17 );#   
//
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Field.h"

int main()
{
    using namespace sf; // for graphics
    using namespace std; // for cout, cin

    srand((unsigned int)time(NULL)); // for random numbers
    
    string input; // input string

    cout << "Welcome! It is a program to construct geometrical objects. Please, enter your code to draw something.\n ";

    cout << "Write your program: \n";
    getline(cin, input);
    char* field_input = new char[input.size() + 1];
    for (int i = 0; i < input.size(); i++)
        field_input[i] = input[i];
    field_input[input.size()] = '\0';
    Field field(field_input);

    cout << field.getLexErrors() << endl;              // prints the errors of the lexer
    cout << field.getSyntErrors() << endl;			  // prints the errors of the parser

#ifndef NDEBUG
        const std::vector<Field::Point>& points = field.getPoints();
        const std::vector<Field::Line>& lines = field.getLines();
        const std::vector<Field::Segment>& segments = field.getSegments();

        cout << "Result:\n";

        for (const Field::Point& p : points)
            cout << p << endl;

        for (const Field::Line& l : lines)
            cout << l << endl;

        for (const Field::Segment& s : segments)
            cout << s << endl;
#endif // !NDEBUG


        field.drawAll();

    return 0;
}


