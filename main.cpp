// It is the attempt to create a kind of compiler for geometrical obeject;

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Field.h"

int main()
{
    using namespace sf;
    using namespace std;

    while (true) {
        string input;

        cout << "Welcome! It is a program to construct geometrical objects. See the rules which define how to write the program to construct something.\n ";
        cout << "Rules:\n";
        // TODO: write rules.


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

        cout << "Result:\n";

        for (Field::Point p : points)
            cout << p << endl;

        for (Field::Line l : lines)
            cout << l << endl;
    }

    
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


