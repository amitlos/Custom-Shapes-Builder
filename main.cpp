// It is the attempt to create a kind of compiler for geometrical obeject;
// Examples of the program code:
//DRAW_POINTS A, B, C; DRAW_LINES a, b, c;# 
//DRAW_POINTS A, B, C; DRAW_LINES a(A, C), b, c;CONNECT A,B; CONNECT B,C; MARK_POINTS K(1,1) ON AB; MARK_POINTS L(3,1) ON AB; MARK_POINTS P(1,2) ON BC;# 
// 
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

        cout << "Enter your program: \n";
        getline(cin, input);
        char* field_input = new char[input.size() + 1];
        for (int i = 0; i < input.size(); i++)
            field_input[i] = input[i];
        field_input[input.size()] = '\0';
        Field field(field_input);

        cout << field.getLexErrors() << endl;
        cout << field.getSyntErrors() << endl;

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

        field.drawAll();

    

    
    // Lexer tester
    /*
    char ch = 'y';
    while (ch == 'y')
    {
        string input;
        getline(cin,input);
        char* lex_input = new char[input.size() + 1];
        for (int i = 0; i < input.size(); i++)
            lex_input[i] = input[i];
        lex_input[input.size()] = '\0';
        Lexer lex(lex_input);

        while (!lex.isEmpty())
        {
            cout << lex.getToken() << endl;
        }
        cout << "Would you like to continue?\n";
        cin >> ch;
    }
    */

    return 0;
}


