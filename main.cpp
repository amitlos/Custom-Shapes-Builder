// It is the attempt to create a kind of compiler for geometrical obeject;
// Examples of the program code:
//DRAW_POINTS A, B, C; DRAW_LINES a, b, c;# 
//DRAW_POINTS A, B, C; DRAW_LINES a(A, C), b, c, d, u, o ,l, k;CONNECT A,B; CONNECT B,C; MARK_POINTS K(1,1) ON AB; MARK_POINTS L(3,1) ON AB; MARK_POINTS P(1,2) ON BC;# 
// dvd
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

    cout << "Welcome! It is a program to construct geometrical objects. See the rules which define how to write the program to construct something.\n ";
    cout << "Rules:\n";
    // TODO: write rules.
    // TODO: write examples.

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

        for (Field::Point p : points)
            cout << p << endl;

        for (Field::Line l : lines)
            cout << l << endl;

        for (Field::Segment s : segments)
            cout << s << endl;
#endif // !NDEBUG


        field.drawAll();

    return 0;
}


