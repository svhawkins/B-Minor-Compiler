/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_SOURCE_PARSER_H_INCLUDED
# define YY_YY_SOURCE_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
typedef enum yytokentype
  {
    TOKEN_ERROR = 258,
    TOKEN_EOF = 259,
    TOKEN_ARRAY = 260,
    TOKEN_AUTO = 261,
    TOKEN_BOOLEAN = 262,
    TOKEN_CHAR = 263,
    TOKEN_ELSE = 264,
    TOKEN_FOR = 265,
    TOKEN_FUNCTION = 266,
    TOKEN_IF = 267,
    TOKEN_INTEGER = 268,
    TOKEN_PRINT = 269,
    TOKEN_RETURN = 270,
    TOKEN_STRING = 271,
    TOKEN_VOID = 272,
    TOKEN_WHILE = 273,
    TOKEN_IDENT = 274,
    TOKEN_BOOL = 275,
    TOKEN_NUMBER = 276,
    TOKEN_STR = 277,
    TOKEN_CH = 278,
    TOKEN_INC = 279,
    TOKEN_DEC = 280,
    TOKEN_SUB = 281,
    TOKEN_EXP = 282,
    TOKEN_NOT = 283,
    TOKEN_MULT = 284,
    TOKEN_DIV = 285,
    TOKEN_MOD = 286,
    TOKEN_ADD = 287,
    TOKEN_LEQ = 288,
    TOKEN_LESS = 289,
    TOKEN_GEQ = 290,
    TOKEN_GREAT = 291,
    TOKEN_EQ = 292,
    TOKEN_NEQ = 293,
    TOKEN_AND = 294,
    TOKEN_OR = 295,
    TOKEN_ASSIGN = 296,
    TOKEN_LBRACK = 297,
    TOKEN_RBRACK = 298,
    TOKEN_LPAR = 299,
    TOKEN_RPAR = 300,
    TOKEN_LCURL = 301,
    TOKEN_RCURL = 302,
    TOKEN_COLON = 303,
    TOKEN_SEMI = 304,
    TOKEN_COMMA = 305
  } token_t;
#endif

/* Value type.  */


//extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SOURCE_PARSER_H_INCLUDED  */
