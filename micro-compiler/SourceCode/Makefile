parser:
	bison --yacc -dv parser.y

scanner:
	flex scanner.l

compiler:
	g++ -o compiler y.tab.c lex.yy.c generator.cpp

all:
	make parser
	make scanner
	make compiler

clean:
	rm -f compiler lex.yy.c y.output y.tab.c y.tab.h