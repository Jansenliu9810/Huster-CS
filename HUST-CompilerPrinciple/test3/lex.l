%option yylineno

%{
#include <stdio.h>
#include "parser.tab.h"
#include "string.h"
#include "def.h"

int yycolumn=1;
#define YY_USER_ACTION    yylloc.first_line=yylloc.last_line=yylineno; \
	yylloc.first_column=yycolumn;\
    yylloc.last_column=yycolumn+yyleng-1;\
    yycolumn+=yyleng;
typedef union {
	int type_int;
	int type_float;
    char type_char;
	char type_id[32];
	struct node *ptr;
} YYLVAL;
#define YYSTYPE YYLVAL
/*
enum Token{
    INT = 1,CHAR,FLOAT,INCLUDE,TYPE,STRUCT,ID,
    IF,ELSE,WHILE,FOR,BREAK,CONTION,RETURN,VOID,
    RELOP,ASSIGNOP,PLUS,MINUS,STAR,DIV,SELFADD,SELFSUB,ADD_ASSIGNOP,MINUS_ASSIGNOP,STAR_ASSIGNOP,DIV_ASSIGNOP,AND,OR,NOT,SL,SR,
    SEMI,COMMA,LP,RP,LC,RC,LB,RB,DOT
};

char *tokenStr[] = {
    " ","INT","CHAR","FLOAT","INCLUDE","TYPE","STRUCT","ID",
    "IF","ELSE","WHILE","FOR","BREAK","CONTION","RETURN","VOID",
    "RELOP","ASSIGNOP","PLUS","MINUS","STAR","DIV","SELFADD","SELFSUB","ADD_ASSIGNOP","MINUS_ASSIGNOP","STAR_ASSIGNOP","DIV_ASSIGNOP","AND","OR","NOT","SL","SR",
    "SEMI","COMMA","LP","RP","LC","RC","LB","RB","DOT"
};
*/
%}

id  [_A-Za-z][_A-Za-z0-9]*
int ([1-9][0-9]*)|0
float ([0-9]*\.[0-9]+)|([0-9]+\.)
char \'[0-9A-Za-z]\'
string (\".*\")
include ("#include "[<\"][_a-zA-Z][_0-9a-zA-Z]*"."[_a-zA-Z][_0-9a-zA-Z]*[>\"]"")

%%
{int}       {yylval.type_int=atoi(yytext);return INT;}
{float}     {yylval.type_float=atoi(yytext);return FLOAT;}
{char}      {yylval.type_char=atoi(yytext);return CHAR;}

"int"       {strcpy(yylval.type_id,yytext);return TYPE;}       //匹配保留字 
"float"     {strcpy(yylval.type_id,yytext);return TYPE;}
"char"      {strcpy(yylval.type_id,yytext);return TYPE;}
"srting"    {strcpy(yylval.type_id,yytext);return TYPE;}
"struct"    {strcpy(yylval.type_id,yytext);return STRUCT;}

"if"        {return IF;}
"else"      {return ELSE;}
"while"     {return WHILE;}
"for"       {return FOR;}
"break"     {return BREAK;}
"continue"  {return CONTINUE;}
"return"    {return RETURN;}

{id}        {strcpy(yylval.type_id,yytext);return ID;}    /*标识符的匹配要在保留字后面，保留字也满足标识符的格式*/

">"|"<"|">="|"<="|"=="|"!=" {strcpy(yylval.type_id,yytext);return RELOP;} //匹配操作符
"="	    	{return ASSIGNOP;}
"+"		{return PLUS;}
"-"		{return MINUS;}
"*"		{return STAR;}
"/"		{return DIV;}
"++"        	{return SELFADD;}
"--"        	{return SELFSUB;}
"+="        	{return ADD_ASSIGNOP;}
"-="        	{return MINUS_ASSIGNOP;}
"*="        	{return STAR_ASSIGNOP;}
"/="        	{return DIV_ASSIGNOP;}
"&&"        	{return AND;}
"||"        	{return OR;}
"!"		{return NOT;}

";"         	{return SEMI;}  //匹配界符
","         	{return COMMA;}
"("         	{return LP;}
")"         	{return RP;}
"{"         	{return LC;}
"}"         	{return RC;}
"["         	{return LB;}
"]"         	{return RB;}
"."         	{return DOT;}

[ \r\t]     {}
[\n]        {yycolumn=1;}

\/\/[^\n]*  {;}//匹配单行注释
\/\*(\s|.)*?\*\/ {;}//匹配多行注释

.			{printf("\nError at line %-3d: %s\n\n",yylineno,yytext);}//无效符号
%%
/*
void main(int argc, char* args){
    int token;
    while(token = yylex()){
        printf("(%s, \"%s\")\n",tokenStr[token],yytext);
    }
}
*/

int yywrap()
{
    return 1;
}