#include <string>
#include<stack>
#include<vector>
#include<list>

using namespace std;

class Calculator {
    virtual float Calculate( string expression ){return 0;};
};

class IStringable {
public:
    virtual string getText( ){return "";}
};

class DijkstraStackNode  {
public:
    DijkstraStackNode( ) {}
    DijkstraStackNode(string text) { name = text; }
    virtual ~DijkstraStackNode(){};
    string ToString( ) {
        return name;
    }
    string TypeName( ) {
        return type;
    }
protected:
    string name;
    string type;
};

#define operation "operation"
class AripthmeticOperation : public DijkstraStackNode {
public:
    AripthmeticOperation(){ priority = 0; type = operation; }
    AripthmeticOperation( string sign, int weight ) {
        name = sign;
        priority = weight;
        type = operation;
    }
    virtual float func( float first, float second ){ return 0;}
    int Priority( ) {
        return priority;
    }    
protected:   
    int priority;
};

class AddOperaion : public AripthmeticOperation {
public:
    AddOperaion( int priority ) {
        name= "+";
        priority = priority;
    }
    float func( float  first, float second ) {
        return first + second;
    }    
};

class SubOperaion : public AripthmeticOperation {
public:
    SubOperaion( int priority ) {
        name = "-";
        priority = priority;
    }
    float func( float  first, float second ) {
        return first - second;
    }
};

class DivOperaion : public AripthmeticOperation {
public:
    DivOperaion( int priority ) {
        name = "/";
        priority = priority;
    }
    float func( float  first, float second ) {
        return first / second;
    }
};

class MulOperaion : public AripthmeticOperation {
public:
    MulOperaion( int priority ) {
        name = "*";
        priority = priority;
    }
    float func( float  first, float second ) {
        return first * second;
    }
};

#define argument "argument"
class Argument : public DijkstraStackNode {
public:
    Argument( ) {
        number = 0;
        type = argument;
    }
    Argument( float data ) {
        number = data;
        name = to_string( data );
        type = argument;
    }
    Argument( float data, string text ) {
        number = data;
        name = text;
        type = argument;
    }
    string ToString( ) {
        return to_string( number );
    }
    float value( ) {
        return number;
    }
private:
    float number;
};

#define bracket "bracket"
class OpeningBracket : public DijkstraStackNode {
public:
    OpeningBracket( ) {
        name = "(";
        type = bracket;
    }
};


class DijkstraCalc : public Calculator, public IStringable {
public:
    float Calculate( string expression );
    string ToString( ) {
        return reversePolishNotaion;
    }
    string getText( ) {
        char out[256] = {0};
        sprintf_s( out, 256, "Converted: %s = %.3f", reversePolishNotaion.c_str(), result );
        return string(out);
    }
    DijkstraCalc( ) {
        reversePolishNotaion = "";
        result = 0;
        text = "";
        memset( priority, 0, 256);
        priority['('] = 1;
        priority[')'] = 2;
        priority['+'] = 3;
        priority['-'] = 4;
        priority['*'] = 5;
        priority['/'] = 5;
        priority[':'] = 5;
    }
private:
    int ParseExpression( string expression );
    int take_data_from_parse_buffer( string& parse_buffer );
    void pop_operations( char coperation );
    int count();
    //vector<DijkstraStackNode*> CountStack;

    list<DijkstraStackNode*> CountStack;
    vector<DijkstraStackNode*> my_stack;
    string reversePolishNotaion;
    float result;
    string text;
    char priority[256];

};

class FormatException :  public IStringable {
public: 
    FormatException( string error ){ text = error; }
    /*const char* what( ) const throw () {
        return text.c_str();
    }*/
    string getText( ) {
        return text;
    }
    string text;
};
