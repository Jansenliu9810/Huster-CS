%{
    enum yytokentype {
     INT = 1,
     ID = 2,
     RELOP = 3,
     TYPE = 4,
     FLOAT = 5,
     LP = 6,
     RP = 7,
     LC = 8,
     RC = 9,
     SEMI = 10,
     COMMA = 11,
     PLUS = 12,
     MINUS = 13,
     STAR = 14,
     DIV = 15,
     ASSIGNOP = 16,
     AND = 17,
     OR = 18,
     NOT = 19,
     IF = 20,
     ELSE = 21,
     WHILE = 22,
     RETURN = 23,
     UMINUS = 24,
     LOWER_THEN_ELSE = 25,
     DOUBLE_Q = 26,
     EXACT_DIVI = 27,
     CHAR = 28,
     STRING = 29,
     FOR = 30,
     BREAK = 31,
     CONTINUE = 32,
     SWITCH = 33,
     CASE = 34,
     STRUCT = 35,
     DEFAULT = 36,
     AUTOPLUS = 37,
     AUTOMINUS = 38,
     PLUSASSIGNOP = 39,
     MINUSASSIGNOP = 40,
     LB = 41,
     RB = 42,
     DO = 43,
     SIZEOF = 44,
     SCANF = 45,
     PRINTF = 46,
     VOID = 47,
     BACKLASH = 48,
     SINGLE_Q = 49,
     DOT = 50,
     POUND = 51,
     COMMENTBEGIN = 52,
     COMMENTEND = 53,
     COMMENT = 54,
     ASSIGNMENT = 55,
   };

char *tokenStr[] = {
    "INT", "ID", "RELOP", "TYPE", "FLOAT", "LP", "RP",
     "LC", "RC", "SEMI", "COMMA", "PLUS", "MINUS", "STAR",
     "DIV", "ASSIGNOP", "AND", "OR", "NOT", "IF", "ELSE",
     "WHILE","RETURN","UMINUS","LOWER_THEN_ELSE","DOUBLE_Q",
     "EXACT_DIVI","CHAR","STRING","FOR","BREAK","CONTINUE","SWITCH","CASE",
     "STRUCT","DEFAULT","AUTOPLUS","AUTOMINUS","PLUSASSIGNOP","MINUSASSIGNOP",
     "LB","RB","DO","SIZEOF","SCANF","PRINTF","VOID","BACKLASH","SINGLE_Q",
     "DOT","POUND","COMMENTBEGIN","COMMENTEND","COMMENT","ASSIGNMENT",
};


%}
%option yylineno

id     [A-Za-z][A-Za-z0-9]*  
int    [0-9]+
float  ([0-9]*\.[0-9]+)|([0-9]+\.)
char   (\'[^\n]\')
string (\"[^\n]\")



%%
{int}       {return INT;}
{float}     {return FLOAT;}
{char}		{return CHAR;}
{string}	{return STRING;}
"int"       {return TYPE;}
"float"     {return TYPE;}
"char"		{return TYPE;}
"string"	{return TYPE;}


"return"    {return RETURN;}
"if"        {return IF;}
"else"      {return ELSE;}
"while"     {return WHILE;}
"do"		{return DO;}
"for"		{return FOR;}
"break"		{return BREAK;}
"continue"	{return CONTINUE;}
"switch"	{return SWITCH;}
"case"		{return CASE;}
"struct"	{return STRUCT;}
"default"	{return DEFAULT;}
"sizeof"	{return SIZEOF;}
"scanf"		{return SCANF;}
"printf"	{return PRINTF;}
"void"		{return VOID;}


{id}       	{return ID;}
"\//"		{return COMMENT;}

"\/*"		{return COMMENTBEGIN;}
"\*/"		{return COMMENTEND;}

"."			{return DOT;}
";"			{return SEMI;}
","			{return COMMA;}
"#"			{return POUND;}
">"|"<"|">="|"<="|"=="|"!=" {return RELOP;}
"="			{return ASSIGNOP;}
"+"			{return PLUS;}
"++"		{return AUTOPLUS;}
"+="		{return PLUSASSIGNOP;}
"-"			{return MINUS;}
"--"		{return AUTOMINUS;}
"-="		{return MINUSASSIGNOP;}
"*"			{return STAR;}
"/"			{return DIV;}
"&&"        {return AND;}
"&"			{return ASSIGNMENT;}
"||"        {return OR;}
"!"			{return NOT;}
"("			{return LP;}
")"			{return RP;}
"{"			{return LC;}
"}"			{return RC;}
"["			{return LB;}
"]"			{return RB;}
"\""	    {return DOUBLE_Q;}
"\'"		{return SINGLE_Q;}
"\\"		{return BACKLASH;}
"%"			{return EXACT_DIVI;}
[\n]        {}   
[ \r\t]     {}
\/\/[^\n]*  {}//匹配注释
.	{printf("Error type A :Mysterious character \"%s\"\n\t at Line %d\n",yytext,yylineno);}

%%


/*
int main(int argc, char** argv) {
    if (argc >1) {
        if  (!(yyin = fopen(argv[1],"r"))) {
            perror(argv[1]);
            return 1;
         }
        int symbol;
        while(symbol = yylex()){
            printf("(%s, \"%s\")\n",tokenStr[symbol-1],yytext);
        }

   } else printf("请给出要分析的源程序名\n");
   return 0;
}
*/
int yywrap()
{
return 1;
}
