bison --yacc -dv parser.y
flex scanner.l		
g++ -o compiler y.tab.c lex.yy.c generator.cpp
@REM start compiler.exe