/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
     ASS = 258,
     PLUS = 259,
     MINUS = 260,
     DIV = 261,
     MUL = 262,
     CONS = 263,
     FUNDEF = 264,
     OPE = 265,
     MINS = 266,
     PSTFIX = 267,
     INT = 268,
     RETURN = 269,
     IF = 270,
     ELSE = 271,
     WHILE = 272,
     EQU = 273,
     NEQU = 274,
     ELT = 275,
     LT = 276,
     EMT = 277,
     MT = 278,
     OR = 279,
     AND = 280,
     CMPD = 281,
     CONSTANT = 282,
     IDENTIFIER = 283
   };
#endif
/* Tokens.  */
#define ASS 258
#define PLUS 259
#define MINUS 260
#define DIV 261
#define MUL 262
#define CONS 263
#define FUNDEF 264
#define OPE 265
#define MINS 266
#define PSTFIX 267
#define INT 268
#define RETURN 269
#define IF 270
#define ELSE 271
#define WHILE 272
#define EQU 273
#define NEQU 274
#define ELT 275
#define LT 276
#define EMT 277
#define MT 278
#define OR 279
#define AND 280
#define CMPD 281
#define CONSTANT 282
#define IDENTIFIER 283




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 8 "parser.y"
{
		int i;
		char *str;
		tree n;
}
/* Line 1529 of yacc.c.  */
#line 111 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

