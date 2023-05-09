%{
    #include<string.h>
    #include <iostream>
    #include <vector>
    #include <string>
    #include "generator.h"
    #include <stdio.h>
    #include "y.tab.h"
    int yyerror(const char * s);
    int yylex();

%}
%option noyywrap


%%


--.*\n                        {}
\n                            {}
\t                            {}

"begin"                       { return _BEGIN;}
"end"                         { return _END;}//terminate here?
"read"                        { return _READ;}
"write"                       { return _WRITE;}
"("                           { return _LPAREN;}
")"                           { return _RPAREN;}
";"                           { return _SEMICOLON;}
","                           { return _COMMA;}
":="                          { return _ASSIGN_OP;}
"+"                           { return _PLUOP;}
"-"                           { return _MINUSOP;}
[a-zA-Z][a-zA-Z0-9]*          { sscanf(yytext,"%s",yylval.id_struct); return _ID;}
([0-9])+                      { yylval.int_struct = atoi(yytext); return _INTLITERAL;}



<<EOF>>                       { return _SCANEOF; yyterminate();}
.      


%%
// int main(int argc, char **argv)
// {
//   yylex();
//   int x;
//   std::cin>>x;
//   yywrap();
// }
// int yywrap()
// {
// 	return 1;
// }
