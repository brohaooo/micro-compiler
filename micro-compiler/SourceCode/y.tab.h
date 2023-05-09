/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    _BEGIN = 258,
    _END = 259,
    _READ = 260,
    _WRITE = 261,
    _LPAREN = 262,
    _RPAREN = 263,
    _SEMICOLON = 264,
    _COMMA = 265,
    _ASSIGN_OP = 266,
    _PLUOP = 267,
    _MINUSOP = 268,
    _SCANEOF = 269,
    _INTLITERAL = 270,
    _ID = 271
  };
#endif
/* Tokens.  */
#define _BEGIN 258
#define _END 259
#define _READ 260
#define _WRITE 261
#define _LPAREN 262
#define _RPAREN 263
#define _SEMICOLON 264
#define _COMMA 265
#define _ASSIGN_OP 266
#define _PLUOP 267
#define _MINUSOP 268
#define _SCANEOF 269
#define _INTLITERAL 270
#define _ID 271

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 13 "parser.y" /* yacc.c:1909  */

    char id_struct[32];
    int int_struct;
    struct EXPR expr_struct;
    struct EXPR_LIST expr_list_struct;
    struct PRIM prim_struct;
    struct ID_LIST id_list_struct;
    char add_op_struct;

#line 96 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
