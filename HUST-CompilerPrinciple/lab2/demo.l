%{
int num_id = 0;
enum symbol {
    invalid=1,         ident,     number,     plus,      minus,
    times,       slash,     oddsym,     eql,       neq,
    lss,         leq,       gtr,        geq,       lparen,
    rparen,      comma,     semicolon,  period,    becomes,
    beginsym,    endsym,    ifsym,      thensym,   whilesym,
    writesym,    readsym,   dosym,      callsym,   constsym,
    varsym,      procsym
};

char *tokenStr[] = {
    "null",  "invalidword",     "ident",     "number",     "plus",      "minus",
    "times",     "slash",     "oddsym",     "eql",       "neq",
    "lss",         "leq",       "gtr",        "geq",       "lparen",
   "rparen",     "comma",     "semicolon",  "period",    "becomes",
    "beginsym",    "endsym",    "ifsym",      "thensym",   "whilesym",
    "writesym",    "readsym",   "dosym",      "callsym",   "constsym",
    "varsym",      "procsym"
};

%}
digit  [0-9]
letter [A-Za-z]
id {letter}({letter}|{digit})*  
invalid {digit}+{letter}+{digit}*
number {digit}+[^A-Za-z]
%%
{invalid} {return invalid;}
{number} {return number;}
"=" {return eql;}
"#" {return neq;}
"<" {return lss;}
"<=" {return leq;}
">" {return gtr;}
">=" {return geq;}
":=" {return becomes;}
"+" {return plus;}
"-" {return minus;}
"*" {return times;}
"/" {return slash;}

(odd) {return oddsym;}

(const) {return constsym;}
"var" {return varsym;}
"procedure" {return procsym;}
"begin" {return beginsym;}
"end" {return endsym;}
 "if" {return ifsym;}
"then" {return thensym;}
"while" {return whilesym;}
"do" {return dosym;}
"call" {return callsym;}
"read" {return readsym;}
"write" {return writesym;}

"(" {return lparen;}
")" {return rparen;}
";" {return semicolon;}
"," {return comma;}
"." {return period;}

{id} {++num_id; return ident;}
[ \n\t]+ {}

%%

int yywrap() 
{ 
   return(1); 
} 

int main(int argc, char** argv) {
    if (argc >1) {
        if  (!(yyin = fopen(argv[1],"r"))) {
            perror(argv[1]);
            return 1;
         }

        int symbol;
        while(symbol = yylex()){
            printf("(%s, \"%s\")\n",tokenStr[symbol],yytext);
        }

       printf("# of id = %d\n", num_id);

   } else printf("请给出要分析的源程序名\n");
   return 0;
}