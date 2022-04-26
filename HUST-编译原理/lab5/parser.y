%error-verbose  /*指示bison生成详细的错误消息*/
%locations      /*记录行号*/
%{
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "def.h"
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
int yylex();
void yyerror(const char* fmt, ...);
void display(struct ASTNode *,int);
%}

%union {
	int    type_int;
	float  type_float;
        char   type_char;
	char   type_id[32];
	struct ASTNode *ptr;
};

/*
%type定义非终结符的语义值类型 %type <union 的成员名>  非终结符
%type  <ptr> program ExtDefList 
这表示非终结符 ExtDefList 属性值的类型对应联合中成员 ptr 的类型，在本实验中对应一个树结点的指针
*/
%type  <ptr> program ExtDefList ExtDef Specifier StructSpecifier ExtDecList FuncDec ArrayDec CompSt VarList VarDec ParamDec Stmt StmList DefList Def DecList Dec Exp Args

//% token 定义终结符的语义值类型
/*%token <type_id> ID，表示识别出来一个标识符后，标识符的字符串串值保存在成员 type_id */
%token <type_int> INT              /*指定INT的语义值是type_int，有词法分析得到的数值*/
%token <type_id> ID  RELOP TYPE    /*指定ID,RELOP,TYPE 的语义值是type_id，有词法分析得到的标识符字符串*/
%token <type_float> FLOAT          
%token <type_char> CHAR

%token LP RP LB RB LC RC SEMI COMMA 
%token ASSIGNOP PLUS MINUS STAR DIV AND OR DOT NOT STRUCT RETURN BREAK CONTINUE IF ELSE WHILE FOR ADD_ASSIGNOP MINUS_ASSIGNOP STAR_ASSIGNOP DIV_ASSIGNOP
       SELFADD SELFSUB


//优先级
%left ADD_ASSIGNOP MINUS_ASSIGNOP STAR_ASSIGNOP DIV_ASSIGNOP
%left ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right UMINUS NOT SELFADD SELFSUB
%left DOT LP RP LB RB 

/*%nonassoc的含义是没有结合性,它一般与%prec结合使用表示该操作有同样的优先级*/
%nonassoc LOWER_THEN_ELSE
%nonassoc ELSE

%%
//规则部分
/*Exp:  Exp ASSIGNOP Exp {$$=mknode(ASSIGNOP,$1,$3,NULL,yylineno); } */
/*规则后面{}中的是当完成归约时要执行的语义动作。规则左部的 Exp 的属性值用$$表示，右部有 2 个 Exp，位置序号分别是 1 和 3，其属性值分别用$1 和$3表示

Stmt(statement) → error SEMI 表示对语句分析时，一旦有错，跳过分号（SEMI），继续进行语法分析*/


program: ExtDefList { display($1,0); semantic_Analysis0($1);} /*显示语法树,语义分析*/
         ;
         /*因为semantic_Analysis0($1);是语义分析，实验一暂时用不到*/


/*program: ExtDefList    { display($1,0); }     //归约到program，开始显示语法树,语义分析
         ; */
/*ExtDefList：外部定义列表，即是整个语法树*/
ExtDefList: {$$=NULL;}
          | ExtDef ExtDefList {$$=mknode(2,EXT_DEF_LIST,yylineno,$1,$2);}   //每一个EXTDEFLIST的结点，其第1棵子树对应一个外部变量声明或函数
          ;  
/*外部声明，声明外部变量或者声明函数*/
ExtDef:   Specifier ExtDecList SEMI   {$$=mknode(2,EXT_VAR_DEF,yylineno,$1,$2);}   //该结点对应一个外部变量声明
         |Specifier FuncDec CompSt    {$$=mknode(3,FUNC_DEF,yylineno,$1,$2,$3);}         //该结点对应一个函数定义
         |Specifier ArrayDec SEMI  {$$=mknode(2,ARRAY_DEF,yylineno,$1,$2);}      //数组定义
         | error SEMI   {$$=NULL;printf("---缺少分号---\n");}
         |Specifier SEMI {$$=mknode(1,EXT_VAR_DEF,yylineno,$1);}
         ;
/*表示一个type类型，int、float、char、string, 或者struct*/
Specifier:  TYPE    {
                        $$=mknode(0,TYPE,yylineno);strcpy($$->type_id,$1);
                        if(!strcmp($1,"int")) $$->type = INT;
                        else if(!strcmp($1,"float")) $$->type = FLOAT;
                        else if(!strcmp($1,"char")) $$->type = CHAR;
                    }
        | StructSpecifier {$$ = $1;}
                    ;
/*表示一个结构体*/
StructSpecifier: STRUCT ID LC DefList RC {$$=mknode(1,STRUCT,yylineno,$4);strcpy($$->type_id,$2);}
        |        STRUCT ID {$$=mknode(1,STRUCT,yylineno,$2);strcpy($$->type_id,$2);}
        |        STRUCT LC DefList RC {$$=mknode(1,STRUCT,yylineno,$3);}
        ;
/*变量名称列表，由一个或多个变量组成，多个变量之间用逗号隔开*/
ExtDecList:  VarDec      {$$=$1;}       /*每一个EXT_DECLIST的结点，其第一棵子树对应一个变量名(ID类型的结点),第二棵子树对应剩下的外部变量名*/
           | VarDec COMMA ExtDecList {$$=mknode(2,EXT_DEC_LIST,yylineno,$1,$3);}
           ;  
/*变量名称，由一个ID组成*/
VarDec:  ID          {$$=mknode(0,ID,yylineno);strcpy($$->type_id,$1);}   //ID结点，标识符符号串存放结点的type_id
         ;
/*函数名+参数定义*/
FuncDec: ID LP VarList RP   {$$=mknode(1,FUNC_DEC,yylineno,$3);strcpy($$->type_id,$1);}//函数名存放在$$->type_id
	| ID LP  RP   {$$=mknode(0,FUNC_DEC,yylineno);strcpy($$->type_id,$1);$$->ptr[0]=NULL;}//函数名存放在$$->type_id
        | error RP {$$=NULL; printf("---函数左右括号不匹配---\n");}
        ;  
/*数组声明*/
ArrayDec: ID LB Exp RB {$$=mknode(1,ARRAY_DEC,yylineno,$3);strcpy($$->type_id,$1);}
        | ID LB RB {$$=mknode(0,ARRAY_DEC,yylineno);strcpy($$->type_id,$1);}
        | error RB {$$=NULL;printf("---数组定义错误---\n");}
/*参数定义列表，有一个到多个参数定义组成，用逗号隔开*/
VarList: ParamDec  {$$=mknode(1,PARAM_LIST,yylineno,$1);}
        | ParamDec COMMA  VarList  {$$=mknode(2,PARAM_LIST,yylineno,$1,$3);}
        ;
/*参数定义，固定有一个类型和一个变量组成*/    
ParamDec: Specifier VarDec         {$$=mknode(2,PARAM_DEC,yylineno,$1,$2);}
         ;
/*复合语句，左右分别用大括号括起来，中间有定义列表和语句列表*/
CompSt: LC DefList StmList RC    {$$=mknode(2,COMP_STM,yylineno,$2,$3);}
       | error RC {$$=NULL; printf("---复合语句内存在错误---\n");}
       ;
/*语句列表，由0个或多个语句stmt组成*/
StmList: {$$=NULL; }  
        | Stmt StmList  {$$=mknode(2,STM_LIST,yylineno,$1,$2);}
        ;
/*语句，可能为表达式，复合语句，return语句，if语句，if-else语句，while语句，for*/
Stmt:   Exp SEMI    {$$=mknode(1,EXP_STMT,yylineno,$1);}
      | CompSt      {$$=$1;}      //复合语句结点直接作为语句结点，不再生成新的结点
      | RETURN Exp SEMI   {$$=mknode(1,RETURN,yylineno,$2);}
      | IF LP Exp RP Stmt %prec LOWER_THEN_ELSE   {$$=mknode(2,IF_THEN,yylineno,$3,$5);}
      | IF LP Exp RP Stmt ELSE Stmt   {$$=mknode(3,IF_THEN_ELSE,yylineno,$3,$5,$7);}
      | FOR LP Exp SEMI Exp SEMI Exp RP Stmt {$$=mknode(4,FOR,yylineno,$3,$5,$7,$9);}
      | WHILE LP Exp RP Stmt {$$=mknode(2,WHILE,yylineno,$3,$5);}
      ;
/*定义列表，由0个或多个定义语句组成*/
DefList: {$$=NULL; }
        | Def DefList {$$=mknode(2,DEF_LIST,yylineno,$1,$2);}
        ;
/*定义一个或多个语句语句，由分号隔开*/
Def:    Specifier DecList SEMI {$$=mknode(2,VAR_DEF,yylineno,$1,$2);}
        | Specifier ArrayDec SEMI {$$=mknode(2,ARRAY_DEF,yylineno,$1,$2);}
        ;
/*语句列表，由一个或多个语句组成，由逗号隔开，最终都成一个表达式*/
DecList: Dec  {$$=mknode(1,DEC_LIST,yylineno,$1);}
       | Dec COMMA DecList  {$$=mknode(2,DEC_LIST,yylineno,$1,$3);}
	   ;
/*语句，一个变量名称或者一个赋值语句（变量名称等于一个表达式）*/
Dec:     VarDec  {$$=$1;}
       | VarDec ASSIGNOP Exp  {$$=mknode(2,ASSIGNOP,yylineno,$1,$3);strcpy($$->type_id,"ASSIGNOP");}
       ;
/*表达式*/
Exp:    Exp ASSIGNOP Exp {$$=mknode(2,ASSIGNOP,yylineno,$1,$3);strcpy($$->type_id,"ASSIGNOP");}//$$结点type_id空置未用，正好存放运算符
      | Exp AND Exp   {$$=mknode(2,AND,yylineno,$1,$3);strcpy($$->type_id,"AND");}
      | Exp OR Exp    {$$=mknode(2,OR,yylineno,$1,$3);strcpy($$->type_id,"OR");}
      | Exp RELOP Exp {$$=mknode(2,RELOP,yylineno,$1,$3);strcpy($$->type_id,$2);}  //词法分析关系运算符号自身值保存在$2中
      | Exp PLUS Exp  {$$=mknode(2,PLUS,yylineno,$1,$3);strcpy($$->type_id,"PLUS");}
      | Exp MINUS Exp {$$=mknode(2,MINUS,yylineno,$1,$3);strcpy($$->type_id,"MINUS");}
      | Exp STAR Exp  {$$=mknode(2,STAR,yylineno,$1,$3);strcpy($$->type_id,"STAR");}
      | Exp DIV Exp   {$$=mknode(2,DIV,yylineno,$1,$3);strcpy($$->type_id,"DIV");}
      | LP Exp RP     {$$=$2;}
      | MINUS Exp %prec UMINUS   {$$=mknode(1,UMINUS,yylineno,$2);strcpy($$->type_id,"UMINUS");}
      | NOT Exp       {$$=mknode(1,NOT,yylineno,$2);strcpy($$->type_id,"NOT");}
      | Exp SELFADD   {$$=mknode(1,SELFADD_R,yylineno,$1);strcpy($$->type_id,"SELFADD");}
      | SELFADD Exp   {$$=mknode(1,SELFADD_L,yylineno,$2);strcpy($$->type_id,"SELFADD");}
      | Exp SELFSUB   {$$=mknode(1,SELFSUB_R,yylineno,$1);strcpy($$->type_id,"SELFSUB");}
      | SELFSUB Exp   {$$=mknode(1,SELFSUB_L,yylineno,$2);strcpy($$->type_id,"SELFSUB");}
      | Exp ADD_ASSIGNOP Exp  {$$=mknode(2,ADD_ASSIGNOP,yylineno,$1,$3);strcpy($$->type_id,"ADD_ASSIGNOP");}
      | Exp MINUS_ASSIGNOP Exp {$$=mknode(2,MINUS_ASSIGNOP,yylineno,$1,$3);strcpy($$->type_id,"MINUS_ASSIGNOP");}
      | Exp STAR_ASSIGNOP Exp  {$$=mknode(2,STAR_ASSIGNOP,yylineno,$1,$3);strcpy($$->type_id,"STAR_ASSIGNOP");}
      | Exp DIV_ASSIGNOP Exp   {$$=mknode(2,DIV_ASSIGNOP,yylineno,$1,$3);strcpy($$->type_id,"DIV_ASSIGNOP");}

      | Exp DOT Exp   {$$=mknode(2,STRUCT_ACCESS,yylineno,$1,$3);}
      | ID LP Args RP {$$=mknode(1,FUNC_CALL,yylineno,$3);strcpy($$->type_id,$1);}
      | ID LP RP      {$$=mknode(0,FUNC_CALL,yylineno);strcpy($$->type_id,$1);}
      | ID            {$$=mknode(0,ID,yylineno);strcpy($$->type_id,$1);}
      | INT           {$$=mknode(0,INT,yylineno);$$->type_int=$1;$$->type=INT;}
      | FLOAT         {$$=mknode(0,FLOAT,yylineno);$$->type_float=$1;$$->type=FLOAT;}
      | CHAR          {$$=mknode(0,CHAR,yylineno);$$->type_char = $1;}
      | BREAK         {$$=mknode(0,BREAK,yylineno);strcpy($$->type_id,"BREAK");}
      | CONTINUE      {$$=mknode(0,CONTINUE,yylineno);strcpy($$->type_id,"CONTINUE");}
      ;
/*用逗号隔开的参数*/
Args:    Exp COMMA Args    {$$=mknode(2,ARGS,yylineno,$1,$3);}
       | Exp               {$$=mknode(1,ARGS,yylineno,$1);}
       ;
       
%%

int main(int argc, char *argv[]){
  yyin=fopen(argv[1],"r");
  if (!yyin) 
    return 0;
  yylineno=1;
  yyparse();
  return 0;
}

#include<stdarg.h>
void yyerror(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
}
