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
%token <std::string> T_NUM
%token T_TRUE
%token T_FALSE
%token <std::string> T_ID
%token T_INCREMENT
%token T_QUESTIONMARK
%token T_COLON

%left T_OR T_AND
%left T_EQ
%left T_LESS T_GR
%left T_ADD T_SUB
%left T_MUL T_DIV T_MOD
%precedence T_NOT

%type <expression> expression
%type <std::string> statement statements

%%

start:
    T_PROGRAM T_ID declarations T_BEGIN statements T_END
    {
        std::cout << "global main" << std::endl;
        std::cout << "extern read_unsigned, write_unsigned" << std::endl;
        std::cout << "extern read_boolean, write_boolean" << std::endl;
        std::cout << "segment .bss" << std::endl;
        std::map<std::string,symbol_data>::iterator it; 
        for(it = symbol_table.begin(); it != symbol_table.end(); ++it) {
            std::string label = it->second.label;
            std::cout
                << it->second.label << ": "
                << (it->second.typ == boolean ? "resb " : "resd ")
                << 1 << std::endl;
        } 
        std::cout << std::endl;
        std::cout << "segment .text" << std::endl;
        std::cout << "main:" << std::endl;
        std::cout << $5 << std::endl;
        std::cout << "ret" << std::endl;
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
		symbol_table[$2] = symbol_data(integer);
    }
|
    T_BOOLEAN T_ID T_SEMICOLON
    {
		if( symbol_table.count($2) > 0 )
		{
			semantic_error(@1.begin.line, "Re-declared variable: " + $2);
		}
		symbol_table[$2] = symbol_data(boolean);
    }
;

statements:
    statement
    {
        $$ = $1;
    }
|
    statement statements
    {
        $$ = $1 + $2;
    }
;

statement:
    T_SKIP T_SEMICOLON
    {
        $$ = "nop";
    }
|
    T_ID T_ASSIGN expression T_SEMICOLON
    {
		if( symbol_table.count($1) == 0 )
		{
			semantic_error(@1.begin.line, "Undeclared variable: " + $1);
		}
		if(symbol_table[$1].typ != $3.typ)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
        if($3.typ == integer)
            $$ = "" +
                 $3.code +
                 "mov [" + symbol_table[$1].label + "], eax\n";
        if($3.typ == boolean)
            $$ = "" +
                 $3.code +
                 "mov [" + symbol_table[$1].label + "], al\n";
    }
|
    T_READ T_OPEN T_ID T_CLOSE T_SEMICOLON
    {
		if( symbol_table.count($3) == 0 )
		{
			semantic_error(@1.begin.line, "Undeclared variable: " + $3);
		}
        if(symbol_table[$3].typ == integer)
        {
            $$ = "call read_unsigned\nmov [" + symbol_table[$3].label + "], eax\n";
        }
        if(symbol_table[$3].typ == boolean)
        {
            $$ = "call read_boolean\nmov [" + symbol_table[$3].label + "], al\n";
        }
    }
|
    T_WRITE T_OPEN expression T_CLOSE T_SEMICOLON
    {
        if($3.typ == integer)
        {
            $$ = "" +
                 $3.code +
                 "push eax\n" +
                 "call write_unsigned\n" +
                 "add esp,4\n";
        }
        if($3.typ == boolean)
        {
            $$ = "xor eax, eax\n" +
                 $3.code +
                 "push eax\n" +
                 "call write_boolean\n" +
                 "add esp,4\n";
        }
    }
|
    T_IF expression T_THEN statements T_ENDIF
    {
		if($2.typ != boolean)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
        std::string end = next_label();
        $$ = "" +
             $2.code +
             "cmp al, 1\n" +
             "jne near " + end + "\n" +
             $4 +
             end + ":\n";
    }
|
    T_IF expression T_THEN statements T_ELSE statements T_ENDIF
    {
		if($2.typ != boolean)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
        std::string elsel = next_label();
        std::string end = next_label();
        $$ = "" +
             $2.code +
             "cmp al, 1\n" +
             "jne near " + elsel + "\n" +
             $4 +
             "jmp " + end + "\n" +
             elsel + ":\n" +
             $6 +
             end + ":\n";
    }
|
    T_WHILE expression T_DO statements T_DONE
    {
		if($2.typ != boolean)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
        std::string start = next_label();
        std::string end = next_label();
        $$ = "" +
             start + ":\n" +
             $2.code +
             "cmp al, 1\n" +
             "jne near " + end + "\n" +
             $4 +
             "jmp " + start + "\n" +
             end + ":\n";
    }
;

expression:
    T_NUM
    {
		$$ = expression(integer, "mov eax, " + $1 + "\n");
    }
|
    T_TRUE
    {
		$$ = expression(boolean, "mov al, 1\n");
    }
|
    T_FALSE
    {
		$$ = expression(boolean, "mov al, 0\n");
    }
|
    T_ID
    {
		if( symbol_table.count($1) == 0 )
		{
			semantic_error(@1.begin.line, "Undeclared variable: " + $1);
		}
		if(symbol_table[$1].typ == integer)
        {
            $$ = expression(symbol_table[$1].typ,
                    "mov eax, [" + symbol_table[$1].label + "]\n");
        }
        if(symbol_table[$1].typ == boolean)
        {
            $$ = expression(symbol_table[$1].typ,
                    "mov al, [" + symbol_table[$1].label + "]\n");
        }
    }
|
    expression T_ADD expression
    {
		if($1.typ != integer || $3.typ != integer)
		{
		   semantic_error(@1.begin.line, "Type error.");
           
		}
		$$ = expression(integer, "" +
                $3.code +
                "push eax\n" +
                $1.code +
                "pop ebx\n" +
                "add eax, ebx\n");
    }
|
    expression T_SUB expression
    {
		if($1.typ != integer || $3.typ != integer)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = expression(integer, "" +
                $3.code +
                "push eax\n" +
                $1.code +
                "pop ebx\n" +
                "sub eax, ebx\n");
    }
|

    expression T_MUL expression
    {
		if($1.typ != integer || $3.typ != integer)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = expression(integer, "" +
                $3.code +
                "push eax\n" +
                $1.code +
                "pop ebx\n" +
                "mul ebx\n");
    }
|
    expression T_DIV expression
    {
		if($1.typ != integer || $3.typ != integer)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = expression(integer,
                "xor edx, edx\n" +
                $3.code +
                "push eax\n" +
                $1.code +
                "pop ebx\n" +
                "div ebx\n");
    }
|
    expression T_MOD expression
    {
		if($1.typ != integer || $3.typ != integer)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = expression(integer, 
                "xor edx, edx\n" +
                $3.code +
                "push eax\n" +
                $1.code +
                "pop ebx\n" +
                "div ebx\n" +
                "mov eax, edx\n");
    }
|
    expression T_LESS expression
    {
		if($1.typ != integer || $3.typ != integer)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = expression(boolean, "" +
                $3.code +
                "push eax\n" +
                $1.code +
                "pop ebx\n" +
                "cmp eax, ebx\n" +
                "setl al\n");
    }
|
    expression T_GR expression
    {
		if($1.typ != integer || $3.typ != integer)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = expression(boolean, "" +
                $3.code +
                "push eax\n" +
                $1.code +
                "pop ebx\n" +
                "cmp eax, ebx\n" +
                "setg al\n");
    }
|
    expression T_EQ expression
    {
		if($1.typ != $3.typ)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = expression(boolean, "" +
                $3.code +
                "push eax\n" +
                $1.code +
                "pop ebx\n" +
                "cmp eax, ebx\n" +
                "sete al\n");
    }
|
    expression T_AND expression
    {
		if($1.typ != boolean || $3.typ != boolean)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = expression(boolean, "" +
                $3.code +
                "push ax\n" +
                $1.code +
                "pop bx\n" +
                "and al, bl\n");
    }
|
    expression T_OR expression
    {
		if($1.typ != boolean || $3.typ != boolean)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = expression(boolean, "" +
                $3.code +
                "push ax\n" +
                $1.code +
                "pop bx\n" +
                "or al, bl\n");
    }
|
    T_NOT expression
    {
		if($2.typ != boolean)
		{
		   semantic_error(@1.begin.line, "Type error.");
		}
		$$ = expression(boolean, "" +
                $2.code +
                "xor al, 1\n");
    }
|
    T_OPEN expression T_CLOSE
    {
		$$ = expression($2.typ, "" + $2.code);
    }
|
    T_ID T_INCREMENT
{
    if(symbol_table[$1].typ != integer)
    {
        semantic_error(@1.begin.line, "Type error.");
    }
    $$ = expression(integer, std::string("") +
            "mov ax, [" + symbol_table[$1].label + "]\n" +
            "inc ax\n" +
            "mov [" + symbol_table[$1].label + "], ax\n");
}
|
    T_OPEN expression T_QUESTIONMARK expression T_COLON expression T_CLOSE
    {
        if($2.typ != boolean)
        {
            semantic_error(@1.begin.line, "Type error.");
        }
        std::string elseLabel = next_label();
        std::string endLabel = next_label();
        $$ = expression($4.typ, "" +
                $2.code +
                "cmp al, 1\n" +
                "jne near " + elseLabel + "\n" +
                $4.code +
                "jmp " + endLabel + "\n" +
                elseLabel + ":\n" +
                $6.code +
                endLabel + ":\n");
    }
;
