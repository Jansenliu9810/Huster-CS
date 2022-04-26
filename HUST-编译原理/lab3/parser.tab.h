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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
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
    INT = 258,
    ID = 259,
    RELOP = 260,
    TYPE = 261,
    CHAR = 262,
    STRING = 263,
    FLOAT = 264,
    LP = 265,
    RP = 266,
    LB = 267,
    RB = 268,
    LC = 269,
    RC = 270,
    SEMI = 271,
    COMMA = 272,
    STRUCT = 273,
    SWITCH = 274,
    CASE = 275,
    DOT = 276,
    DEFAULT = 277,
    POUND = 278,
    COMMENT = 279,
    COMMENTBEGIN = 280,
    COMMENTEND = 281,
    VOID = 282,
    ASSIGNMENT = 283,
    SIZEOF = 284,
    SCANF = 285,
    PRINTF = 286,
    BACKLASH = 287,
    SINGLE_Q = 288,
    DOUBLE_Q = 289,
    BREAK = 290,
    CONTINUE = 291,
    PLUS = 292,
    MINUS = 293,
    STAR = 294,
    DIV = 295,
    ASSIGNOP = 296,
    AND = 297,
    OR = 298,
    NOT = 299,
    IF = 300,
    ELSE = 301,
    WHILE = 302,
    RETURN = 303,
    FOR = 304,
    DO = 305,
    PLUSASSIGNOP = 306,
    MINUSASSIGNOP = 307,
    AUTOPLUS = 308,
    AUTOMINUS = 309,
    EXT_DEF_LIST = 310,
    EXT_VAR_DEF = 311,
    FUNC_DEF = 312,
    FUNC_DEC = 313,
    EXT_DEC_LIST = 314,
    PARAM_LIST = 315,
    PARAM_DEC = 316,
    VAR_DEF = 317,
    DEC_LIST = 318,
    DEF_LIST = 319,
    COMP_STM = 320,
    STM_LIST = 321,
    EXP_STMT = 322,
    IF_THEN = 323,
    IF_THEN_ELSE = 324,
    FUNC_CALL = 325,
    ARGS = 326,
    FUNCTION = 327,
    PARAM = 328,
    ARG = 329,
    CALL = 330,
    LABEL = 331,
    GOTO = 332,
    JLT = 333,
    JLE = 334,
    JGT = 335,
    JGE = 336,
    EQ = 337,
    NEQ = 338,
    FOR_DEC = 339,
    ARRAY_LIST = 340,
    ARRAY_CALL = 341,
    UMINUS = 342,
    LOWER_THEN_ELSE = 343
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 15 "parser.y"

	int    type_int;
	float  type_float;
        char   type_char;
        char   type_string[31];
	char   type_id[32];
	struct node *ptr;

#line 155 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
