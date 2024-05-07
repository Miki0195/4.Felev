%language "c++"
%locations
%define api.value.type variant

%code requires {
#include "semantics.hh"
}

%code provides {
int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yylloc);
}

%token T_PROGRAM
%token T_BEGIN
%token T_END
%token T_INTEGER 
%token T_BOOLEAN
%token T_SKIP
%token T_IF
%token T_THEN
%token T_ELSE
%token T_ENDIF
%token T_WHILE
%token T_DO
%token T_DONE
%token T_READ
%token T_WRITE
%token T_SEMICOLON
%token T_ASSIGN
%token T_OPEN
%token T_CLOSE
%token T_NUM
%token T_TRUE
%token T_FALSE
%token <std::string> T_ID

%left T_OR T_AND
%left T_EQ
%left T_LESS T_GR
%left T_ADD T_SUB
%left T_MUL T_DIV T_MOD
%precedence T_NOT

%type <type> expression;

%%

start:
    T_PROGRAM T_ID declarations T_BEGIN statements T_END
    {
    }
;

declarations:
    // empty
    {
    }
|
    declaration declarations
    {
    }
;

declaration:
    T_INTEGER T_ID T_SEMICOLON
    {
		if( symbol_table.count($2) > 0 )
		{
			semantic_error(@1.begin.line, "Re-declared variable: " + $2);
		}
		symbol_table[$2] = integer;
    }
|
    T_BOOLEAN T_ID T_SEMICOLON
    {
		if( symbol_table.count($2) > 0 )
		{
			semantic_error(@1.begin.line, "Re-declared variable: " + $2);
		}
		symbol_table[$2] = boolean;
    }
;

statements:
    statement
    {
    }
|
    statement statements
    {
    }
;

statement:
    T_SKIP T_SEMICOLON
    {
    }
|
   T_ID T_ASSIGN expression T_SEMICOLON
    {
		if( symbol_table.count($1) == 0 )
		{
			semantic_error(@1.begin.line, "Undeclared variable: " + $1);
		}
		if(symbol_table[$1] != $3)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
    }
|
    T_READ T_OPEN T_ID T_CLOSE T_SEMICOLON
    {
		if( symbol_table.count($3) == 0 )
		{
			semantic_error(@1.begin.line, "Undeclared variable: " + $3);
		}
    }
|
    T_WRITE T_OPEN expression T_CLOSE T_SEMICOLON
    {
    }
|
    T_IF expression T_THEN statements T_ENDIF
    {
		if($2 != boolean)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
    }
|
    T_IF expression T_THEN statements T_ELSE statements T_ENDIF
    {
		if($2 != boolean)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
    }
|
    T_WHILE expression T_DO statements T_DONE
    {
		if($2 != boolean)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
    }
;

expression:
    T_NUM
    {
		$$ = integer;
    }
|
    T_TRUE
    {
		$$ = boolean;
    }
|
    T_FALSE
    {
		$$ = boolean;
    }
|
    T_ID
    {
		if( symbol_table.count($1) == 0 )
		{
			semantic_error(@1.begin.line, "Undeclared variable: " + $1);
		}
		$$ = symbol_table[$1];
    }
|
    expression T_ADD expression
    {
		if($1 != integer || $3 != integer)
		{
		   semantic_error(@1.begin.line, "Type error.");
           
		}
		$$ = integer;
    }
|
    expression T_SUB expression
    {
		if($1 != integer || $3 != integer)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = integer;
    }
|
    expression T_MUL expression
    {
		if($1 != integer || $3 != integer)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = integer;
    }
|
    expression T_DIV expression
    {
		if($1 != integer || $3 != integer)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = integer;
    }
|
    expression T_MOD expression
    {
		if($1 != integer || $3 != integer)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = integer;
    }
|
    expression T_LESS expression
    {
		if($1 != integer || $3 != integer)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = boolean;
    }
|
    expression T_GR expression
    {
		if($1 != integer || $3 != integer)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = boolean;
    }
|
    expression T_EQ expression
    {
		if($1 != $3)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = boolean;
    }
|
    expression T_AND expression
    {
		if($1 != boolean || $3 != boolean)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = boolean;
    }
|
    expression T_OR expression
    {
		if($1 != boolean || $3 != boolean)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = boolean;
    }
|
    T_NOT expression
    {
		if($2 != boolean)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = boolean;
    }
|
    T_OPEN expression T_CLOSE
    {
		$$ = $2;
    }
;
