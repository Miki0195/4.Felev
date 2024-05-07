%option noyywrap c++ yylineno

%{
#include <iostream>
#include <stdlib.h>
%}

DIGIT  		[0-9]
WS          [ \t\n]
LETTER      [a-zA-Z]

%%

program     std::cout << "T_PROGRAM" << std::endl;
begin       std::cout << "T_BEGIN" << std::endl;
end         std::cout << "T_END" << std::endl;
integer     std::cout << "T_INTEGER" << std::endl; 
boolean     std::cout << "T_BOOLEAN" << std::endl;
skip        std::cout << "T_SKIP" << std::endl;
if          std::cout << "T_IF" << std::endl;
then        std::cout << "T_THEN" << std::endl;
else        std::cout << "T_ELSE" << std::endl;
endif       std::cout << "T_ENDIF" << std::endl;
while       std::cout << "T_WHILE" << std::endl;
do          std::cout << "T_DO" << std::endl;
done        std::cout << "T_DONE" << std::endl;
read        std::cout << "T_READ" << std::endl;
write       std::cout << "T_WRITE" << std::endl;
";"         std::cout << "T_SEMICOLON" << std::endl;
":="        std::cout << "T_ASSIGN" << std::endl;
"="         std::cout << "T_EQ" << std::endl;
"<"         std::cout << "T_LESS" << std::endl;
">"         std::cout << "T_GR" << std::endl;
"+"         std::cout << "T_ADD" << std::endl;
"-"         std::cout << "T_SUB" << std::endl;
"*"         std::cout << "T_MUL" << std::endl;
and         std::cout << "T_AND" << std::endl;
or          std::cout << "T_OR" << std::endl;
not         std::cout << "T_NOT" << std::endl;
div         std::cout << "T_DIV" << std::endl; 
mod         std::cout << "T_MOD" << std::endl;
"("         std::cout << "T_OPEN" << std::endl;
")"         std::cout << "T_CLOSE" << std::endl;
"?"         std::cout << "T_QUESTIONMARK" << std::endl;
":"         std::cout << "T_COLON" << std::endl;
date        std::cout << "T_DATE" << std::endl;

{DIGIT}+("."|,){DIGIT}{2} std::cout << "T_FLOAT" << std::endl; 

{DIGIT}+	std::cout << "T_NUM" << std::endl;
true        std::cout << "T_TRUE" << std::endl;
false       std::cout << "T_FALSE" << std::endl;

"#".*\n    // nothing to do

{LETTER}({LETTER}|{DIGIT}|_)*    std::cout << "T_ID" << std::endl;
_{DIGIT}{1,2}*{LETTER}{2,4}*    std::cout << "T_ID_MODIFIED" << std::endl;

[19|20]\d\d-(0[1-9]|1[012])-(0[1-9]|[12][0-9]|3[01]) std::cout << "T_DATE_LITERAL" << std::endl;

0b(0|1)+    std::cout << "T_BIN" << std::endl;
0o[0-7]+    std::cout << "T_OCT" << std::endl;
0b([0-7a-fA-F])+ std::cout << "T_HEX" << std::endl;

{WS}+   // nothing to do

. {
    std::cerr << "Line " << lineno() << ": Lexical error." << std::endl;
    exit(1);
}

%%
