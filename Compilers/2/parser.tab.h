/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2006, 2009-2010 Free Software
   Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_SELECT = 258,
     TOK_FROM = 259,
     TOK_WHERE = 260,
     TOK_LIMIT = 261,
     TOK_GROUP = 262,
     TOK_ORDER = 263,
     TOK_BY = 264,
     TOK_IN = 265,
     TOK_AND = 266,
     TOK_OR = 267,
     TOK_NOT = 268,
     TOK_CREATE = 269,
     TOK_TABLE = 270,
     TOK_INT_TYPE = 271,
     TOK_FLOAT_TYPE = 272,
     TOK_VARCHAR = 273,
     TOK_EQ = 274,
     TOK_NEQ = 275,
     TOK_GT = 276,
     TOK_LT = 277,
     TOK_GTE = 278,
     TOK_LTE = 279,
     TOK_STAR = 280,
     TOK_IDENT = 281,
     TOK_STR_LIT = 282,
     TOK_INT_LIT = 283,
     TOK_FLOAT_LIT = 284
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 56 "parser.y"

    char  *str;
    int    ival;
    float  fval;
    int    type_tag;   /* ColType value */



/* Line 1685 of yacc.c  */
#line 89 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


