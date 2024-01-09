// It is the attempt to create a kind of compiler for geometrical obeject;
// Examples of the program code:
//DRAW_POINTS A, B, C; DRAW_LINES a, b, c;# 
//DRAW_POINTS A, B, C; DRAW_LINES a(A, C), b, c;CONNECT A,B; CONNECT B,C; MARK_POINTS K(1,1) ON AB; MARK_POINTS L(3,1) ON AB; #
// BUILD_TRIANGLE PKL;  BUILD_TRIANGLE PKL; BUILD_TRIANGLE CAB(90, 4, 3); BUILD_TRIANGLE JHG(120,10, 3 ); BUILD_TRIANGLE ZXC(60, 5, 5); BUILD_TRIANGLE NMK(30, 10, 17 );# 
// DRAW_LINES a((100,0), (100, 100));#
// DRAW_LINES a((100,100), (200, 100));#
// DRAW_POINTS A, B, C(100, 100); DRAW_LINES a, b(A, (100, 200)), c(A,B); CONNECT B,C; MARK_POINTS K (1,1) ON BC; MARK_POINTS L(1,2) ON BC; BUILD_TRIANGLE PKL; BUILD_TRIANGLE CAB(90, 4, 3); BUILD_TRIANGLE ZXC(60, 5, 5);#   
//
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Field.h"
#include "Drawer.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

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
    const std::vector<Point>& points = field.getPoints();
    const std::vector<Line>& lines = field.getLines();
    const std::vector<Segment>& segments = field.getSegments();
    const std::vector<Triangle>& triangles = field.getTriangles();

    cout << "Result:\n";

    for (const Point& p : points)
        cout << p << endl;

    for (const Line& l : lines)
        cout << l << endl;

    for (const Segment& s : segments)
        cout << s << endl;

    for (const Triangle& t : triangles)
        cout << t << endl;
#endif

    RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Custom Shapes Builder");

    Drawer drawer(window);

    while (window.isOpen())
    {
        window.clear(Color::White);

        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        drawer.drawPoints(field.getPoints());
        drawer.drawLines(field.getLines());
        drawer.drawSegments(field.getSegments());
        drawer.drawTriangles(field.getTriangles());

        window.display();
    }

    return 0;
}


