# micro-compiler
a compiler for micro language, implemented by flex and bison

This is a project in CSC4180. It consists of three parts: 

1. scanner (implemented by flex)
2. parser (implemented by bison)
3. code generator (implemented by c++, embedded in parser's reduction process)

The dockerfile is given, illustrated the required environment (c++, flex, bison),
you can compile it either in windows (run make_script_in_windows.bat) or in linux (run Makefile)
then use the command
  ./compiler \$testfile
to see the result.
