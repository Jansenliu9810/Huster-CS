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
    ERRORID = 262,
    STRING = 263,
    FLOAT = 264,
    CHAR = 265,
    DPLUS = 266,
    DMINUS = 267,
    LP = 268,
    RP = 269,
    LB = 270,
    RB = 271,
    LC = 272,
    RC = 273,
    SEMI = 274,
    COMMA = 275,
    PLUS = 276,
    MINUS = 277,
    STAR = 278,
    DIV = 279,
    MOD = 280,
    ASSIGNOP = 281,
    PLUSASSIGNOP = 282,
    MINUSASSIGNOP = 283,
    STARASSIGNOP = 284,
    DIVASSIGNOP = 285,
    MODASSIGNOP = 286,
    AND = 287,
    OR = 288,
    NOT = 289,
    IF = 290,
    ELSE = 291,
    WHILE = 292,
    RETURN = 293,
    FOR = 294,
    SWITCH = 295,
    CASE = 296,
    COLON = 297,
    DEFAULT = 298,
    EXT_DEF_LIST = 299,
    EXT_VAR_DEF = 300,
    FUNC_DEF = 301,
    FUNC_DEC = 302,
    EXT_DEC_LIST = 303,
    PARAM_LIST = 304,
    PARAM_DEC = 305,
    VAR_DEF = 306,
    DEC_LIST = 307,
    DEF_LIST = 308,
    COMP_STM = 309,
    STM_LIST = 310,
    EXP_STMT = 311,
    IF_THEN = 312,
    IF_THEN_ELSE = 313,
    BREAK = 314,
    CONTINUE = 315,
    FUNC_CALL = 316,
    ARGS = 317,
    FUNCTION = 318,
    PARAM = 319,
    ARG = 320,
    CALL = 321,
    LABEL = 322,
    GOTO = 323,
    JLT = 324,
    JLE = 325,
    JGT = 326,
    JGE = 327,
    EQ = 328,
    NEQ = 329,
    DPLUS_AFTER = 330,
    DPLUS_BEFORE = 331,
    DMINUS_AFTER = 332,
    DMINUS_BEFORE = 333,
    DIMENSION = 334,
    DIMENSION_LIST = 335,
    ARRAY = 336,
    ARRAY_DEC = 337,
    ARRAY_CALL = 338,
    ARRAY_DF = 339,
    STRUCT = 340,
    STRUCT_VISIT = 341,
    STRUCT_NEW = 342,
    STRUCT_DEC = 343,
    EXT_STRUCT_DEC = 344,
    CASE_STMT = 345,
    CASE_STMT_LIST = 346,
    DEFAULT_STMT = 347,
    SWITCH_STMT = 348,
    LCOMMA = 349,
    UMINUS = 350,
    DOT = 351,
    LOWER_THEN_ELSE = 352
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 16 "parser.y"

	int    type_int;
	float  type_float;
	char type_char;
	char   type_id[32];
	struct ASTNode *ptr;

#line 163 "parser.tab.h"

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
