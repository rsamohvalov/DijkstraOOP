#include "classes.h"
#include <typeinfo>

using namespace std;


int DijkstraCalc::take_data_from_parse_buffer( string& parse_buffer ) {
    if( !parse_buffer.length( ) ) {
        return 1;
    }
    if( parse_buffer.length() ) {
        float data = ( float ) atof( parse_buffer.c_str() );
        if( data ) {
            if( parse_buffer == "-1" ) {
                CountStack.push_back( new Argument( data, "(-1)" ));
            }
            else {
                CountStack.push_back( new Argument(data, parse_buffer ) );
            }
        }
        else {
            return -1;
        }
        reversePolishNotaion += CountStack.back()->ToString() + ' ';
    }
    parse_buffer = "";
    return 0;
}


void DijkstraCalc::pop_operations( char coperation ) {
    while (my_stack.size() ) {
        DijkstraStackNode* data = my_stack.back();
        if( priority[( unsigned char )(data->ToString().c_str()[0])] >= priority[(unsigned char)coperation] ) {
            my_stack.pop_back();
            CountStack.push_back( data );
            reversePolishNotaion += ((AripthmeticOperation*)data)->ToString() + ' ';
        }
        else {
            break;
        }
    }
}

unsigned char isDigit( char letter ) {
    if( letter >= 0x30 && letter <= 0x39 ) {
        return 1;
    }
    return 0;
}

int DijkstraCalc::ParseExpression( string s ) {
    int closing_bracket = 0;
    string parse_buffer;
    size_t input_buffer_index = 0;

    string expression = "";
    while( input_buffer_index < s.length( ) ) {
        if( s.at( input_buffer_index ) != ' ' ) {
            expression += s.at( input_buffer_index );
        }
        input_buffer_index++;
    }
    input_buffer_index = 0;
    while( input_buffer_index < expression.length() ) {
        if( expression.at( input_buffer_index ) == 0x0A || expression.at( input_buffer_index ) == 0x0D ) {
            break;
        }
        if( isDigit(expression.at(input_buffer_index)) || expression.at(input_buffer_index) == ',' || expression.at(input_buffer_index) == '.' ) {
            if( expression.at( input_buffer_index ) == ',' ) {
                parse_buffer += '.';
            }
            else {
                parse_buffer += expression.at(input_buffer_index);
            }
            input_buffer_index++;
            closing_bracket = 0;
            continue;
        }
        switch( expression.at(input_buffer_index) ) {
            case '-': {
                if( !parse_buffer.length() && !closing_bracket ) {                    
                    parse_buffer = "-1";
                    if( take_data_from_parse_buffer( parse_buffer ) ) {
                        throw FormatException( "Format error: unexpected -" );
                    }
                    my_stack.push_back( new MulOperaion( priority['*'] ));
                    input_buffer_index++;
                    continue;
                }
                else {
                    if( !closing_bracket && take_data_from_parse_buffer( parse_buffer) ) {
                        throw FormatException( "Format error: expected argument" );
                    }
                    pop_operations( expression.at( input_buffer_index ) );
                    my_stack.push_back( new SubOperaion( priority[expression.at(input_buffer_index)] ) );
                    closing_bracket = 0;                    
                }
                break;
            }
            case '+': {
                if( !parse_buffer.length() && !closing_bracket ) {
                    input_buffer_index++;
                    continue;
                }
                else {
                    if( !closing_bracket && take_data_from_parse_buffer( parse_buffer ) ) {
                        throw FormatException( "Format error: expected argument" );
                    }
                    pop_operations( expression.at( input_buffer_index ) );
                    my_stack.push_back( new AddOperaion( priority[expression.at(input_buffer_index)] ) );
                    closing_bracket = 0;
                }
                break;
            }
            case '*': {
                if( !closing_bracket && take_data_from_parse_buffer( parse_buffer ) ) {
                    throw FormatException( "Format error: expected argument" );
                }
                pop_operations( expression.at( input_buffer_index ) );
                my_stack.push_back( new MulOperaion( priority[expression.at(input_buffer_index)] ) );
                closing_bracket = 0;
                break;
            }
            case ':':
            case '/': {
                if( !closing_bracket && take_data_from_parse_buffer( parse_buffer ) ) {
                    throw FormatException( "Format error: expected argument" );
                }
                pop_operations( expression.at( input_buffer_index ) );
                my_stack.push_back( new DivOperaion( priority[expression.at(input_buffer_index)] ) );
                closing_bracket = 0;
                break;
            }
            case '(': {
                if( (input_buffer_index < expression.length( ) - 1) &&
                        ( expression.at( input_buffer_index + 1) != '(' &&
                          expression.at( input_buffer_index + 1 ) != '-' &&
                          expression.at( input_buffer_index + 1 ) != '+' &&
                          !isDigit( expression.at( input_buffer_index + 1 ) )
                        )
                  ) {
                    throw FormatException( "Format error: expected argument" );
                }
                my_stack.push_back( new OpeningBracket() );
                closing_bracket = 0;
                break;
            }
            case ')': {
                if( !closing_bracket ) {
                    if( take_data_from_parse_buffer( parse_buffer ) ) {
                        throw FormatException( "Format error: expected argument" );
                    }
                }
                closing_bracket = 1;
                while( CountStack.size() ) {
                    DijkstraStackNode* data = my_stack.back();
                    my_stack.pop_back();
                    if( data->ToString() == "(" ) {
                        break;
                    }
                    CountStack.push_back( data );
                    reversePolishNotaion += (( AripthmeticOperation* ) data)->ToString( ) + ' ';
                }
                break;
            }
            default: {
                throw FormatException( "format error: unexpected symbol " /* +  expression.at( input_buffer_index )*/ );
            }
        }
        input_buffer_index++;
    }

    if( parse_buffer.size() ) {
        if( take_data_from_parse_buffer( parse_buffer ) ) {
            throw FormatException( "Error in format" );
        }
    }

    while( my_stack.size() ) {
        DijkstraStackNode* data = my_stack.back();
        my_stack.pop_back();
        if( data->ToString() == "(" ) {
            throw FormatException( "format error: unexpected (" );
        }
        CountStack.push_back( data );
        reversePolishNotaion += (( AripthmeticOperation* ) data)->ToString( ) + ' ';
    }

    return 0;
}

int DijkstraCalc::count() {
    int ret = 0;
    result = 0;
    if( CountStack.size() <= 1 ) {
        return 0;
    }
    DijkstraStackNode* data = CountStack.back();
    CountStack.pop_back();
    if( data->TypeName() == operation) {
        DijkstraStackNode* first = CountStack.back();
        if( first->TypeName() == argument ) {
            CountStack.pop_back();
            if( CountStack.size() ) {
                DijkstraStackNode* second = CountStack.back();
                if( second->TypeName() == argument ) {
                    CountStack.pop_back();
                    result = ((AripthmeticOperation*)data)->func( ((Argument*)second)->value(), ((Argument*)first)->value() );
                    delete second;
                }
                else {
                    ret = count();
                    if( ret ) {
                        return ret; 
                    }
                    if( CountStack.size() ) {
                        second = CountStack.back();
                        CountStack.pop_back();
                        result = (( AripthmeticOperation* ) data)->func( (( Argument* ) second)->value( ), (( Argument* ) first)->value( ) );
                        delete second;
                    }    
                }
            }
            delete first;
            }
            else {
                ret = count( );
                if( ret ) {
                    return -1;
                }
                if( CountStack.size() ) {
                    first = CountStack.back();
                    CountStack.pop_back();
                    if( CountStack.size() ) {
                        DijkstraStackNode* second = CountStack.back();
                        if( second->TypeName() == argument ) {
                            CountStack.pop_back();
                            result = (( AripthmeticOperation* ) data)->func( (( Argument* ) second)->value( ), (( Argument* ) first)->value( ) );
                            delete second;
                        }
                        else {
                            ret = count( );
                            if( ret ) {
                                return -1;
                            }
                            if( CountStack.size() ) {
                                second = CountStack.back();
                                CountStack.pop_back();
                                result = (( AripthmeticOperation* ) data)->func( (( Argument* ) second)->value( ), (( Argument* ) first)->value( ) );
                                delete second;
                            }
                        }
                    }
                }
                delete first;
            }
            Argument* currentResult = new Argument( result );
            CountStack.push_back( currentResult );       
    }
    delete data;
    return 0;
}


float DijkstraCalc::Calculate( string expression ) {
    if( ParseExpression( expression ) ) {
        throw FormatException( "format error" );
    }
    count();
    if( CountStack.size( ) == 1 ) {
        DijkstraStackNode* node = CountStack.back();
        CountStack.pop_back();
        if( node->TypeName() == argument ) {
            result = ((Argument*)node)->value();
        }
        else {
            throw FormatException( "count error" );
        }
        delete node;
    }
    return result;
}