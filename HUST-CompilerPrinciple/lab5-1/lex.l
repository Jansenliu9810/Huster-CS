%{
#include "parser.tab.h"
#include "string.h"
#include "def.h"
int yycolumn=1;
#define YY_USER_ACTION    	yylloc.first_line=yylloc.last_line=yylineno; \
	yylloc.first_column=yycolumn;	yylloc.last_column=yycolumn+yyleng-1; yycolumn+=yyleng;
typedef union {
	int type_int;
	int type_float;
	char type_char;
	char type_id[32];
	struct node *ptr;
} YYLVAL;
#define YYSTYPE YYLVAL

%}
%option yylineno
ID   [A-Za-z_][A-Za-z0-9_]*
INT  [0-9]+
FLOAT  ([0-9]*\.[0-9]+)|([0-9]+\.)
CHAR [']([^\n])[']
%%

{INT}        	{printf("%d [INT,%s]\n",yylineno,yytext);yylval.type_int=atoi(yytext); return INT;}
{FLOAT}      	{printf("%d [FLOAT,%s]\n",yylineno,yytext);yylval.type_float=atof(yytext); return FLOAT;}
{CHAR}		{printf("%d [CHAR,%s]\n",yylineno,yytext);yylval.type_char=*(yytext+1); return CHAR;}
"int"        	{printf("%d [TYPE,%s]\n",yylineno,yytext);strcpy(yylval.type_id,  yytext);return TYPE;}
"float"      	{printf("%d [TYPE,%s]\n",yylineno,yytext);strcpy(yylval.type_id,  yytext);return TYPE;}
"char"		{printf("%d [TYPE,%s]\n",yylineno,yytext);strcpy(yylval.type_id,  yytext);return TYPE;}
"return"     	{printf("%d [RETURN,%s]\n",yylineno,yytext);return RETURN;}
"if"         	{printf("%d [IF,%s]\n",yylineno,yytext);return IF;return IF;}
"else"       	{printf("%d [ELSE,%s]\n",yylineno,yytext);return ELSE;}
"while"      	{printf("%d [WHILE,%s]\n",yylineno,yytext);return WHILE;}
"for"		{printf("%d [FOR,%s]\n",yylineno,yytext);return FOR;}
"break"		{printf("%d [BREAK,%s]\n",yylineno,yytext);return BREAK;}
"continue" 	{printf("%d [CONTINUE,%s]\n",yylineno,yytext);return CONTINUE;}
"switch" 	{printf("%d [SWITCH,%s]\n",yylineno,yytext);return SWITCH;}//switch
"case"	 	{printf("%d [CASE,%s]\n",yylineno,yytext);return CASE;}//case
"struct" 	{printf("%d [STRUCT,%s]\n",yylineno,yytext);return STRUCT;}//struct
"default" 	{printf("%d [DEFAULT,%s]\n",yylineno,yytext);return DEFAULT;}//DEFAULT

{ID}       		{printf("%d [ID,%s]\n",yylineno,yytext);strcpy(yylval.type_id,  yytext); return ID;/*由于关键字的形式也符合标识符的规则，所以把关键字的处理全部放在标识符的前面，优先识别*/}
":"		 	{printf("%d [COLON,%s]\n",yylineno,yytext);return COLON;}
";"			{printf("%d [SEMI,%s]\n",yylineno,yytext);return SEMI;}
","			{printf("%d [COMMA,%s]\n",yylineno,yytext);return COMMA;}
">"|"<"|">="|"<="|"=="|"!=" 	{printf("%d [RELOP,%s]\n",yylineno,yytext);strcpy(yylval.type_id, yytext);;return RELOP;}
"="			{printf("%d [ASSIGNOP,%s]\n",yylineno,yytext);return ASSIGNOP;}
"+"			{printf("%d [PLUS,%s]\n",yylineno,yytext);return PLUS;}
"-"			{printf("%d [MINUS,%s]\n",yylineno,yytext);return MINUS;}
"*"			{printf("%d [STAR,%s]\n",yylineno,yytext);return STAR;}
"/"			{printf("%d [DIV,%s]\n",yylineno,yytext);return DIV;}
"."      		{printf("%d [DOT,%s]\n",yylineno,yytext);return DOT;}
"%"      		{printf("%d [MOD,%s]\n",yylineno,yytext);return MOD;}
"++"			{printf("%d [DPLUS,%s]\n",yylineno,yytext);return DPLUS;}
"--"			{printf("%d [DMINUS,%s]\n",yylineno,yytext);return DMINUS;}
"+="     		{printf("%d [PLUSASSIGNOP,%s]\n",yylineno,yytext);return PLUSASSIGNOP;}
"-="     		{printf("%d [MINUSASSIGNOP,%s]\n",yylineno,yytext);return MINUSASSIGNOP;}
"*="     		{printf("%d [STARASSIGNOP,%s]\n",yylineno,yytext);return STARASSIGNOP;}
"/="     		{printf("%d [DIVASSIGNOP,%s]\n",yylineno,yytext);return DIVASSIGNOP;}
"%="     		{printf("%d [MODASSIGNOP,%s]\n",yylineno,yytext);return MODASSIGNOP;}
"&&"        		{printf("%d [AND,%s]\n",yylineno,yytext);return AND;}
"||"            	{printf("%d [OR,%s]\n",yylineno,yytext);return OR;}
"!"			{printf("%d [NOT,%s]\n",yylineno,yytext);return NOT;}
"("			{printf("%d [LP,%s]\n",yylineno,yytext);return LP;}
")"			{printf("%d [RP,%s]\n",yylineno,yytext);return RP;}
"["			{printf("%d [LB,%s]\n",yylineno,yytext);return LB;}
"]"			{printf("%d [RB,%s]\n",yylineno,yytext);return RB;}
"{"			{printf("%d [LC,%s]\n",yylineno,yytext);return LC;}
"}"			{printf("%d [RC,%s]\n",yylineno,yytext);return RC;}
[\n]           		{yycolumn=1;}   
[ \r\t]          	{}   
.			{printf("Error type A :Mysterious character \"%s\"\n\t at Line %d\n",yytext,yylineno);}
%%

/* 和bison联用时，不需要这部分
void main()
{
yylex();
return 0;
}

*/
int yywrap()
{
return 1;
}