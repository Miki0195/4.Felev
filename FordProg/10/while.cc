#include <iostream>
#include <fstream>
#include <cstdlib>
#include "FlexLexer.h"
#include "while.tab.hh"

yyFlexLexer *lexer;

int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yylloc) {
    yylloc->begin.line = lexer->lineno();
    int token = lexer->yylex();
    if(token == yy::parser::token::T_ID || token == yy::parser::token::T_NUM) {
        yylval->build(std::string(lexer->YYText()));
    }
    return token;
}

void yy::parser::error(const location_type& loc, const std::string& msg) {
    std::cerr << "Line " << loc.begin.line << ": " << msg << std::endl;
    exit(1);
}

using namespace std;

void input_handler( ifstream& in, int argc, char* argv[] );

int main( int argc, char* argv[] )
{
    ifstream in;
    input_handler( in, argc, argv );
    lexer = new yyFlexLexer(&in);
    yy::parser parser;
    parser.parse();
    delete lexer;
    return 0;
}

void input_handler( ifstream& in, int argc, char* argv[] )
{
    if( argc < 2 )
    {
        cerr << "One command line argument expected." << endl;
        exit(1);
    }
    in.open( argv[1] );
    if( !in )
    {
        cerr << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
}
