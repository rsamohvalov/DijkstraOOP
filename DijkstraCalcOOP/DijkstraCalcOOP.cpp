#include <iostream>
#include "classes.h"

using namespace std;


void PrintText( IStringable* from ) {
    cout << "Error: " << from->getText( ).c_str() << endl;
}

int main()
{
   while( 1 ) {
        DijkstraCalc calc;
        string expression = ""; 
        cout << endl << "Type expression, \"quit\" to exit" << endl;
        getline( cin, expression );
        if( !expression.size( ) ) {
            cout << "empty expression\n";
            continue;
        }
        if( expression == "quit" ) {
            break;
        }
        try {
            calc.Calculate( expression );
            PrintText( &calc );
        }
        catch( FormatException ex ) {
           PrintText( &ex );
        }
    }
}

