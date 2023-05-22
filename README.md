# micro-compiler
a compiler for micro language, implemented by flex and bison

## What is it
This is a project in CSC4180 (it might be useful to you if you are also taking this course). 
I think my implementation is quite straight forward, with a lot of comments, 
so give me star if your like it plz :)
It consists of three parts: 

1. scanner (implemented by flex)
2. parser (implemented by bison)
3. code generator (implemented by c++, embedded in parser's reduction process)

## How to use
The dockerfile is given, illustrated the required environment (c++, flex, bison),
you can compile it either in windows (run make_script_in_windows.bat) or in linux (run Makefile)
then use the command

  ./compiler \$testfile
  
to see the result.

## some issues
This micro-compiler can generate correct MIPS codes, but it doesn't behave like common compilers. 
It uses stack for intermediate values and ID values storage but won't free stack memory during the run.

## final score:
100/100
