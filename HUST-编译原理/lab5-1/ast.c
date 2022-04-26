#include "def.h"
#include "parser.tab.h"
#define DEBUG 1
int LEV=0;
char break_label[30];
char continue_label[30];
char case_temp[30];
char case_label[30];
char array_name[30];
char struct_name[33];

int struct_flag = 0;
int array_index = 0;
int struct_var_flag = 0;
int rtn, flag = 0;
int rtn2;
int return_flag = 0;
struct ASTNode* left;
struct ASTNode* right;
struct ASTNode* mknode(int num, int kind, int pos, ...) {
	struct ASTNode* T = (struct ASTNode*)calloc(sizeof(struct ASTNode), 1);
	int i = 0;
	T->kind = kind;
	T->pos = pos;
	va_list pArgs;
	va_start(pArgs, pos);
	for (i = 0; i < num; i++)
		T->ptr[i] = va_arg(pArgs, struct ASTNode*);
	while (i < 4) T->ptr[i++] = NULL;
	va_end(pArgs);
	return T;
}

int calArrayNums(struct ASTNode* T)
{
	if (T)
	{
		if (T->ptr[1] == 0)
		{
			return T->ptr[0]->type_int;
		}
		return T->ptr[0]->type_int * calArrayNums(T->ptr[1]);
	}
	else
		return 1;
}

/**
 * 填充数组的维度
 */
int fillArray(struct ASTNode* T, int* array, int index)
{
	if (index == 10)
		return -1;
	if (T->ptr[1] == 0)
	{
		array[index] = T->ptr[0]->type_int;
		return 1;
	}
	else
	{
		array[index] = T->ptr[0]->type_int;
		return fillArray(T->ptr[1], array, index + 1);
	}
}

//计算数组每一维度的宽度
int calArrayPerWidth(int* array, int index)
{
	int res = 1;
	while (array[index] != 0 && index < 10)
	{
		res *= array[index];
		index++;
	}
	return res;
}

//计算数组所有维度的宽度
int calArrayWidth(struct ASTNode* T, int* array, int index)
{
	if (T->ptr[1] == 0)
	{
		return T->ptr[0]->type_int;
	}
	if (T->ptr[0])
		return (T->ptr[0]->type_int) * calArrayPerWidth(array, index + 1) + calArrayWidth(T->ptr[1], array, index + 1);
	else
	{
		return 1;
	}
}

void display(struct ASTNode* T, int indent)
{//对抽象语法树的先根遍历
	int i = 1;
	struct ASTNode* T0;
	if (T)
	{
		switch (T->kind) {
		case EXT_DEF_LIST:  display(T->ptr[0], indent + 3);    //显示该外部定义（外部变量和函数）列表中的第一个
			display(T->ptr[1], indent + 3);    //显示该外部定义列表中的其它外部定义
			break;
		case EXT_VAR_DEF:   printf("%*c外部变量定义：(%d)\n", indent, ' ', T->pos);
			display(T->ptr[0], indent + 3);        //显示外部变量类型
			printf("%*c变量名：\n", indent + 3, ' ');
			display(T->ptr[1], indent + 6);        //显示变量列表
			break;
		case TYPE:          printf("%*c类型： %s\n", indent, ' ', T->type_id);
			break;
		case EXT_DEC_LIST:  display(T->ptr[0], indent);     //依次显示外部变量名，
			display(T->ptr[1], indent);     //后续还有相同的，仅显示语法树此处理代码可以和类似代码合并
			break;
		case FUNC_DEF:      printf("%*c函数定义：(%d)\n", indent, ' ', T->pos);
			display(T->ptr[0], indent + 3);      //显示函数返回类型
			display(T->ptr[1], indent + 3);      //显示函数名和参数
			display(T->ptr[2], indent + 3);      //显示函数体
			break;
		case FUNC_DEC:      printf("%*c函数名：%s\n", indent, ' ', T->type_id);
			if (T->ptr[0]) {
				printf("%*c函数形参：\n", indent, ' ');
				display(T->ptr[0], indent + 3);  //显示函数参数列表
			}
			else printf("%*c无参函数\n", indent + 3, ' ');
			break;
		case PARAM_LIST:    display(T->ptr[0], indent);     //依次显示全部参数类型和名称，
			display(T->ptr[1], indent);
			break;
		case PARAM_DEC:
		{
			char* ckind;
			if (T->ptr[0]->type == INT) ckind = "int";
			else if (T->ptr[0]->type == FLOAT)  ckind = "float";
			else if (T->ptr[0]->type == CHAR)  ckind = "char";
			printf("%*c类型：%s, 参数名：%s\n", indent, ' ', T->ptr[0]->type == INT ? "int" : "float", T->ptr[1]->type_id);
			break;
		}
		case EXP_STMT:      printf("%*c表达式语句：(%d)\n", indent, ' ', T->pos);
			display(T->ptr[0], indent + 3);
			break;
		case RETURN:        printf("%*c返回语句：(%d)\n", indent, ' ', T->pos);
			display(T->ptr[0], indent + 3);
			break;
		case COMP_STM:      printf("%*c复合语句：(%d)\n", indent, ' ', T->pos);
			printf("%*c复合语句的变量定义部分：\n", indent + 3, ' ');
			display(T->ptr[0], indent + 6);      //显示定义部分
			printf("%*c复合语句的语句部分：\n", indent + 3, ' ');
			display(T->ptr[1], indent + 6);      //显示语句部分
			break;
		case STM_LIST:      display(T->ptr[0], indent);      //显示第一条语句
			display(T->ptr[1], indent);        //显示剩下语句
			break;
		case WHILE:         printf("%*c循环语句：(%d)\n", indent, ' ', T->pos);
			printf("%*c循环条件：\n", indent + 3, ' ');
			display(T->ptr[0], indent + 6);      //显示循环条件
			printf("%*c循环体：(%d)\n", indent + 3, ' ', T->pos);
			display(T->ptr[1], indent + 6);      //显示循环体
			break;
		case FOR:printf("%*cFOR循环语句：(%d)\n", indent, ' ', T->pos);
			printf("%*c循环变量定义：\n", indent + 3, ' ');
			display(T->ptr[0], indent + 6);
			printf("%*c循环条件：\n", indent + 3, ' ');
			display(T->ptr[1], indent + 6);
			printf("%*c更新语句：\n", indent + 3, ' ');
			display(T->ptr[2], indent + 6);
			printf("%*c循环体：(%d)\n", indent + 3, ' ', T->pos);
			display(T->ptr[3], indent + 6);      //显示循环体
		case BREAK:     printf("%*cbreak中断返回语句：(%d)\n", indent, ' ', T->pos); break;
		case CONTINUE:     printf("%*cCONTINUE跳出继续循环语句：(%d)\n", indent, ' ', T->pos); break;
		case IF_THEN:       printf("%*c条件语句(IF_THEN)：(%d)\n", indent, ' ', T->pos);
			printf("%*c条件：\n", indent + 3, ' ');
			display(T->ptr[0], indent + 6);      //显示条件
			printf("%*cIF子句：(%d)\n", indent + 3, ' ', T->pos);
			display(T->ptr[1], indent + 6);      //显示if子句
			break;
		case IF_THEN_ELSE:  printf("%*c条件语句(IF_THEN_ELSE)：(%d)\n", indent, ' ', T->pos);
			printf("%*c条件：\n", indent + 3, ' ');
			display(T->ptr[0], indent + 6);      //显示条件
			printf("%*cIF子句：(%d)\n", indent + 3, ' ', T->pos);
			display(T->ptr[1], indent + 6);      //显示if子句
			printf("%*cELSE子句：(%d)\n", indent + 3, ' ', T->pos);
			display(T->ptr[2], indent + 6);      //显示else子句
			break;
		case DEF_LIST:      
			display(T->ptr[0], indent);    //显示该局部变量定义列表中的第一个
			display(T->ptr[1], indent);    //显示其它局部变量定义
			break;
		case VAR_DEF:       printf("%*c局部变量定义：(%d)\n", indent, ' ', T->pos);
			display(T->ptr[0], indent + 3);   //显示变量类型
			display(T->ptr[1], indent + 3);   //显示该定义的全部变量名
			break;
		case DEC_LIST:
			printf("%*c变量名：\n", indent, ' ');
			T0 = T;
			while (T0) {
//				printf("%d\n", T0->ptr[0]->kind);
				if (T0->ptr[0]->kind == ID)
					printf("%*c %s\n", indent + 6, ' ', T0->ptr[0]->type_id);
				else if (T0->ptr[0]->kind == ASSIGNOP)
				{
					if (T0->ptr[0]->ptr[0]->kind == ARRAY_DF)
						display(T0->ptr[0]->ptr[0], indent);
					printf("%*c %s ASSIGNOP\n ", indent + 6, ' ', T0->ptr[0]->ptr[0]->type_id);
					display(T0->ptr[0]->ptr[1], indent + strlen(T0->ptr[0]->ptr[0]->type_id) + 7);        //显示初始化表达式
				}
				else if (T0->ptr[0]->kind == ARRAY_DF) {
					//printf("%*c 数组 %s\n", indent + 6, ' ', T0->ptr[0]->type_id);
					display(T0->ptr[0], indent);
				}
				T0 = T0->ptr[1];
			}
			break;
		case ID:	        printf("%*cID： %s\n", indent, ' ', T->type_id);
			break;
		case INT:	        printf("%*cINT：%d\n", indent, ' ', T->type_int);
			break;
		case FLOAT:	        printf("%*cFLAOT：%f\n", indent, ' ', T->type_float);
			break;
		case CHAR:	        printf("%*cCHAR：%c\n", indent, ' ', T->type_char);
			break;
		case ARRAY_CALL:
			printf("%*cID： %s\n", indent, ' ', T->type_id);
			printf("%*c数组下标：\n", indent, ' ');
			display(T->ptr[0], indent);
			break;
		case ARRAY_DF:
			printf("%*c数组 %s\n", indent, ' ', T->type_id);
			display(T->ptr[0], indent + 3);
			printf("\n");
			break;
		case ARRAY_DEC:
			if (T->ptr[0])
				if(T->ptr[0]->kind==ID)
					printf("%*c[%s]\n", indent ,' ',T->ptr[0]->type_id);
				else
					printf("%*c[%d]\n", indent, ' ', T->ptr[0]->type_int);
			display(T->ptr[1], indent);
			break;
		case ARRAY:printf("%*c%c\n", indent, ' ', '{');
			display(T->ptr[0], indent + 3);
			printf("%*c%c\n", indent, ' ', '}');
			break;
		case ASSIGNOP:
		case AND:
		case OR:
		case RELOP:
		case PLUS:
		case MINUS:
		case STAR:
		case DIV:
			printf("%*c%s\n", indent, ' ', T->type_id);
			display(T->ptr[0], indent + 3);
			display(T->ptr[1], indent + 3);
			break;
		case DPLUS:
		case DMINUS:
		case NOT:
		case UMINUS:    printf("%*c%s\n", indent, ' ', T->type_id);
			display(T->ptr[0], indent + 3);
			break;
		case FUNC_CALL: printf("%*c函数调用：(%d)\n", indent, ' ', T->pos);
			printf("%*c函数名：%s\n", indent + 3, ' ', T->type_id);
			display(T->ptr[0], indent + 3);
			break;
		case ARGS:      i = 1;
			while (T) {  //ARGS表示实际参数表达式序列结点，其第一棵子树为其一个实际参数表达式，第二棵子树为剩下的
				struct ASTNode* T0 = T->ptr[0];
				printf("%*c第%d个实际参数表达式：\n", indent, ' ', i++);
				display(T0, indent + 3);
				T = T->ptr[1];
			}
			//                    printf("%*c第%d个实际参数表达式：\n",indent,' ',i);
			  //                  display(T,indent+3);
			printf("\n");
			break;
		case SWITCH_STMT:
			printf("%*cSwitch语句：(%d)\n", indent, ' ', T->pos);
			display(T->ptr[0], indent + 3);
			display(T->ptr[1], indent + 3);
			break;
		case CASE_STMT:
			printf("%*cCase语句：(%d)\n", indent, ' ', T->pos);
			display(T->ptr[0], indent + 6);
			display(T->ptr[1], indent + 6);
			break;
		case DEFAULT_STMT:
			printf("%*cDefualt语句:\n", indent, ' ');
			printf("%*cDefault语句块:\n", indent + 3, ' ');
			display(T->ptr[0], indent + 6);
			break;
		case STRUCT_NEW:
			printf("%*c定义结构体:\n", indent, ' ');
			display(T->ptr[0], indent + 6);
			printf("%*c结构体成员:\n", indent, ' ');
			display(T->ptr[1], indent + 6);
			break;
		case STRUCT_DEC:
			printf("%*c新建结构体变量:\n", indent, ' ');
			printf("%*c   变量名:%s\n", indent, ' ', T->type_id);
			break;
		case EXT_STRUCT_DEC:
			printf("%*c外部结构体:\n", indent, ' ');
			display(T->ptr[0], indent + 3);
			break;
		case STRUCT_VISIT:
			printf("%*c访问对象:\n", indent, ' ');
			display(T->ptr[0], indent + 3);
			printf("%*c目标元素:\n", indent, ' ');
			printf("%*cID: %s\n", indent + 3, ' ', T->type_id);
			break;
		default:
			printf("未找到该类型\n");
			break;
		}
	}
}
char* strcat0(char* s1, char* s2) {
	static char result[10];
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

char* newAlias() {
	static int no = 1;
	char s[10];
	//itoa(no++, s, 10);
	snprintf(s, 10, "%d", no++);
	return strcat0("v", s);
}

char* newLabel() {
	static int no = 1;
	char s[10];
	//itoa(no++, s, 10);
	snprintf(s, 10, "%d", no++);
	return strcat0("label", s);
}

char* newTemp() {
	static int no = 1;
	char s[10];
	//	itoa(no++, s, 10);
	snprintf(s, 10, "%d", no++);
	return strcat0("temp", s);
}

//这里可以只收集错误信息，最后一次显示
void semantic_error(int line, char* msg1, char* msg2) {
	printf("在%d行,%s %s\n", line, msg1, msg2);
}

void callArray(struct ASTNode* T) {
	if (T) {
		switch (T->kind) {
		case INT:
			if (T->type_int <= 0) {
				semantic_error(T->pos, "", "数组下标不能为负值或0");
				break;
			}
			break;
		case FLOAT:
		case CHAR:
			semantic_error(T->pos, "", "数组下标表达式非法");
			break;
		case ARRAY_CALL:
			Exp(T->ptr[0]);
			Exp(T->ptr[1]);
			callArray(T->ptr[0]);
			callArray(T->ptr[1]);
			break;
		default:
			break;
		}
	}
}

//显示符号表
void prn_symbol() {
	int i = 0;
	//printf("%6s %6s %6s  %6s %4s %6s\n", "变量名", "别 名", "层 号", "类  型", "标记", "偏移量");
	printf("-------------------------------------------------------------------------------------------------\n");
	printf("|                                       Symbol Table                                            |\n");
	printf("-------------------------------------------------------------------------------------------------\n");
	printf("|\t%s\t|\t%s\t|\t%s\t|\t%s\t|\t%s\t|\t%s\t|\n", "变量名", "别名", "层号", "类型", "标记", "偏移量");
	printf("-------------------------------------------------------------------------------------------------\n");


	for (i = 0; i < symbolTable.index; ++i) {
		if (!strcmp(symbolTable.symbols[i].name, " "))
			continue;
		printf("|\t%s\t", symbolTable.symbols[i].name);
		printf("|\t%s\t", symbolTable.symbols[i].alias);
		printf("|\t%d\t", symbolTable.symbols[i].level);
		switch (symbolTable.symbols[i].type)
		{
		case INT:
			if (symbolTable.symbols[i].flag != 'A')
				printf("|\t%s\t", "int");
			else
				printf("|\t%s\t", "int[]");
			break;
		case FLOAT:
			if (symbolTable.symbols[i].flag != 'A')
				printf("|\t%s\t", "float");
			else
				printf("|\t%s\t", "float[]");
			break;
		case CHAR:
			if (symbolTable.symbols[i].flag != 'A')
				printf("|\t%s\t", "char");
			else
				printf("|\t%s\t", "char[]");
			break;
		case STRUCT:
			if (symbolTable.symbols[i].flag != 'A')
				printf("|\t%s\t", "struct");
			else
				printf("|\t%s\t", "struct[]");
			break;
		default:
			printf("|\t%s\t", "    ");
			break;
		}
		printf("|\t%c\t", (symbolTable.symbols[i].flag));
		printf("|\t%d\t|\n", symbolTable.symbols[i].offset);
	}
	printf("-------------------------------------------------------------------------------------------------\n");
}

//搜索符号表,看看有没有重名的
int searchSymbolTable(char* name) {
	int i, flag = 0;
	for (i = symbolTable.index - 1; i >= 0; i--) {
		if (!strcmp(symbolTable.symbols[i].name, name))
			return i;
	}
	return -1;
}

//填符号表
int fillSymbolTable(char* name, char* alias, int level, int type, char flag, int offset) {
	//首先根据name查符号表，不能重复定义 重复定义返回-1
	int i;
	/*符号查重，考虑外部变量声明前有函数定义，
	其形参名还在符号表中，这时的外部变量与前函数的形参重名是允许的*/
	for (i = symbolTable.index - 1; i >= 0 && (symbolTable.symbols[i].level == level || level == 0); i--) {
		if (level == 0 && symbolTable.symbols[i].level == 1)
			continue;  //外部变量和形参不必比较重名
		if (!strcmp(symbolTable.symbols[i].name, name))
			return -1;
	}
	//填写符号表内容
	strcpy(symbolTable.symbols[symbolTable.index].name, name);
	strcpy(symbolTable.symbols[symbolTable.index].alias, alias);
	symbolTable.symbols[symbolTable.index].level = level;
	symbolTable.symbols[symbolTable.index].type = type;
	symbolTable.symbols[symbolTable.index].flag = flag;
	symbolTable.symbols[symbolTable.index].offset = offset;
	if (struct_var_flag) {
		strcpy(symbolTable.symbols[symbolTable.index].struct_name, struct_name);
		struct_var_flag = 0;
	}
	return symbolTable.index++; //返回的是符号在符号表中的位置序号，中间代码生成时可用序号取到符号别名
}

//填写临时变量到符号表，返回临时变量在符号表中的位置
int fill_Temp(char* name, int level, int type, char flag, int offset) {
	strcpy(symbolTable.symbols[symbolTable.index].name, "");
	strcpy(symbolTable.symbols[symbolTable.index].alias, name);
	symbolTable.symbols[symbolTable.index].level = level;
	symbolTable.symbols[symbolTable.index].type = type;
	symbolTable.symbols[symbolTable.index].flag = flag;
	symbolTable.symbols[symbolTable.index].offset = offset;
	return symbolTable.index++; //返回的是临时变量在符号表中的位置序号
}

//处理变量列表
void ext_var_list(struct ASTNode* T) {
	int rtn, num = 1;
	switch (T->kind) {
	case EXT_DEC_LIST:
		T->ptr[0]->type = T->type;              //将类型属性向下传递变量结点
		T->ptr[0]->offset = T->offset;          //外部变量的偏移量向下传递
		T->ptr[1]->type = T->type;              //将类型属性向下传递变量结点
		if (T->ptr[0]->kind == ARRAY_DF)
			T->ptr[1]->offset = T->offset + T->width * calArrayNums(T->ptr[0]->ptr[0]);
		else
			T->ptr[1]->offset = T->offset + T->width;
		T->ptr[1]->width = T->width;
		ext_var_list(T->ptr[0]);
		ext_var_list(T->ptr[1]);
		T->num = T->ptr[1]->num + T->ptr[0]->num;
		break;
	case ID:
		rtn = fillSymbolTable(T->type_id, newAlias(), LEV, T->type, 'V', T->offset);  //最后一个变量名
		if (rtn == -1)
			semantic_error(T->pos, T->type_id, "变量重复定义");
		else {
			T->place = rtn;
			T->num = 1;
		}
		break;
	case ARRAY_DF:
		rtn = fillSymbolTable(T->type_id, newAlias(), LEV, T->type, 'A', T->offset);
		if (rtn == -1)
		{
			semantic_error(T->pos, T->type_id, "变量重复定义");
			break;
		}
		struct ASTNode* T0;
		T0 = T->ptr[0];
		while (T0)
		{
			if (T0->ptr[0] && T0->ptr[0]->type_int <= 0 || T0->ptr[0]->type != INT)
			{
				semantic_error(T->pos, T->type_id, "数组下标不能为负值或0");
				return;
			}
			T0 = T0->ptr[1];
		}
		T->place = rtn;
		T->num = calArrayNums(T->ptr[0]);
		break;
	}
}

//对函数实参与形参进行语义检查
int  match_param(int i, struct ASTNode* T) {
	int j, num = symbolTable.symbols[i].paramnum;
	int type1, type2, pos = T->pos;
	T = T->ptr[0];
	if (num == 0 && T == NULL)
		return 1;
	for (j = 1; j <= num; j++) {
		if (!T) {
			semantic_error(pos, "", "函数调用参数太少!");
			return 0;
		}
		type1 = symbolTable.symbols[i + j].type;  //形参类型
		type2 = T->ptr[0]->type;
		if (type1 != type2) {
			semantic_error(pos, "", "参数类型不匹配");
			return 0;
		}
		T = T->ptr[1];
	}
	if (T) { //num个参数已经匹配完，还有实参表达式
		semantic_error(pos, "", "函数调用参数太多!");
		return 0;
	}
	return 1;
}

void boolExp(struct ASTNode* T) {  //布尔表达式，参考文献[2]p84的思想
	struct opn opn1, opn2, result;
	int op;
	int rtn;
	if (T)
	{
		switch (T->kind) {
		case INT:
			T->width = 4;
			break;
		case FLOAT:
			T->width = 8;
			break;
		case CHAR:
			T->width = 1;
			break;
		case ID:
			rtn = searchSymbolTable(T->type_id);
			if (rtn == -1)
				semantic_error(T->pos, T->type_id, "变量为定义");
			if (symbolTable.symbols[rtn].flag == 'F')
				semantic_error(T->pos, T->type_id, "是函数名，类型不匹配");
			else {
				opn1.kind = ID;
				strcpy(opn1.id, symbolTable.symbols[rtn].alias);
				opn1.offset = symbolTable.symbols[rtn].offset;
				opn2.kind = INT;
				opn2.const_int = 0;
				result.kind = ID;
				strcpy(result.id, T->Etrue);
			}
			T->width = 0;
			break;
		case RELOP: //处理关系运算表达式,2个操作数都按基本表达式处理
			T->ptr[0]->offset = T->offset;
			Exp(T->ptr[0]);
			T->width = T->ptr[0]->width;
			T->ptr[1]->offset = T->offset + T->width;
			Exp(T->ptr[1]);
			if (T->width < T->ptr[1]->width) 
				T->width = T->ptr[1]->width;
			opn1.kind = ID; strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
			opn1.offset = symbolTable.symbols[T->ptr[0]->place].offset;
			opn2.kind = ID; strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias);
			opn2.offset = symbolTable.symbols[T->ptr[1]->place].offset;
			result.kind = ID; strcpy(result.id, T->Etrue);
			if (strcmp(T->type_id, "<") == 0)
				op = JLT;
			else if (strcmp(T->type_id, "<=") == 0)
				op = JLE;
			else if (strcmp(T->type_id, ">") == 0)
				op = JGT;
			else if (strcmp(T->type_id, ">=") == 0)
				op = JGE;
			else if (strcmp(T->type_id, "==") == 0)
				op = EQ;
			else if (strcmp(T->type_id, "!=") == 0)
				op = NEQ;
			break;
		case AND:
		case OR:
			T->ptr[0]->offset = T->offset;
			boolExp(T->ptr[0]);
			T->width = T->ptr[0]->width;
			T->ptr[1]->offset = T->offset + T->ptr[0]->width;
			boolExp(T->ptr[1]);
			// if (T->width < T->ptr[1]->width)
			T->width += T->ptr[1]->width;
			break;
		case NOT:   strcpy(T->ptr[0]->Etrue, T->Efalse);
			strcpy(T->ptr[0]->Efalse, T->Etrue);
			boolExp(T->ptr[0]);
			T->code = T->ptr[0]->code;
			break;
		}
	}
}

//处理基本表达式，参考文献[2]p82的思想
void Exp(struct ASTNode* T)
{
	int rtn, num, width;
	struct ASTNode* T0;
	struct opn opn1, opn2, result;
//	printf("%d\n", T->kind);
	if (T)
	{
		switch (T->kind) {
		case ID:
			//查符号表，获得符号表中的位置，类型送type
			rtn = searchSymbolTable(T->type_id);
			if (rtn == -1)
				semantic_error(T->pos, T->type_id, "变量未定义");
			if (symbolTable.symbols[rtn].flag == 'F')
				semantic_error(T->pos, T->type_id, "是函数名，类型不匹配");
			else if (symbolTable.symbols[rtn].flag == 'A')
				semantic_error(T->pos, T->type_id, "是数组变量,类型不匹配");
			else {
				T->place = rtn;       //结点保存变量在符号表中的位置
				T->code = NULL;       //标识符不需要生成TAC
				T->type = symbolTable.symbols[rtn].type;
				T->offset = symbolTable.symbols[rtn].offset;
				T->width = 0;   //未再使用新单元
			}
			break;
		case INT:
			T->place = fill_Temp(newTemp(), LEV, T->type, 'T', T->offset); //为整常量生成一个临时变量
			T->type = INT;
			opn1.kind = INT;
			opn1.const_int = T->type_int;
			result.kind = ID;
			strcpy(result.id, symbolTable.symbols[T->place].alias);
			result.offset = symbolTable.symbols[T->place].offset;
			T->width = 4;
			break;
		case FLOAT:
			T->place = fill_Temp(newTemp(), LEV, T->type, 'T', T->offset);   //为浮点常量生成一个临时变量
			T->type = FLOAT;
			opn1.kind = FLOAT;
			opn1.const_float = T->type_float;
			result.kind = ID;
			strcpy(result.id, symbolTable.symbols[T->place].alias);
			result.offset = symbolTable.symbols[T->place].offset;
			T->width = 8;
			break;
		case CHAR:
			T->place = fill_Temp(newTemp(), LEV, T->type, 'T', T->offset);
			//为字符常量生成一个临时变量
			T->type = CHAR;
			opn1.kind = CHAR;
			opn1.const_char = T->type_char;
			result.kind = ID;
			result.offset = symbolTable.symbols[T->place].offset;
			T->width = 1;
			break;
		case ARRAY_DEC:
				T0 = T;
				while (T0)
				{
					if (T0->ptr[0]->type_int <= 0 || T0->ptr[0]->type != INT)
					{
						semantic_error(T->pos, T->type_id, "数组下标非法");
						return;
					}
					T0 = T0->ptr[1];
				}
				T->place = rtn;
				T->type = symbolTable.symbols[rtn].type;
			break;
		case ASSIGNOP:
			if (T->ptr[0]->kind != ID && T->ptr[0]->kind != ARRAY_DEC && T->ptr[0]->kind != ARRAY_CALL&&T->ptr[0]->kind!=STRUCT_VISIT) {
				semantic_error(T->pos, "", "赋值语句需要左值");
			}
			else {
				Exp(T->ptr[0]);   //处理左值，例中仅为变量
				T->ptr[1]->offset = T->offset;
				Exp(T->ptr[1]);
				if (T->ptr[0]->type != T->ptr[1]->type && T->ptr[0]->kind != ARRAY_CALL) {
					semantic_error(T->pos, "", "赋值号两边的类型不匹配");
					break;
				}
				if (T->ptr[0]->type == CHAR && T->kind != ASSIGNOP) {
					semantic_error(T->pos, T->ptr[0]->type_id, "字符类型变量不能参与运算");
					break;
				}
				if (T->ptr[1]->type == CHAR && T->kind != ASSIGNOP) {
					semantic_error(T->pos, T->ptr[1]->type_id, "字符类型变量不能参与运算");
					break;
				}
				T->type = T->ptr[0]->type;
				T->width = T->ptr[1]->width;
				opn1.kind = ID;
				strcpy(opn1.id, symbolTable.symbols[T->ptr[1]->place].alias);//右值一定是个变量或临时变量
				opn1.offset = symbolTable.symbols[T->ptr[1]->place].offset;
				result.kind = ID;
				strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
				result.offset = symbolTable.symbols[T->ptr[0]->place].offset;
			}
			break;
		case AND:   //按算术表达式方式计算布尔值，未写完
		case OR:    //按算术表达式方式计算布尔值，未写完
		case RELOP: //按算术表达式方式计算布尔值，未写完
			T->type = INT;
			T->ptr[0]->offset = T->ptr[1]->offset = T->offset;
			Exp(T->ptr[0]);
			Exp(T->ptr[1]);
			break;
		case PLUS:
		case MINUS:
		case STAR:
		case DIV:
			T->ptr[0]->offset = T->offset;
			Exp(T->ptr[0]);
			T->ptr[1]->offset = T->offset + T->ptr[0]->width;
			Exp(T->ptr[1]);
			if (T->ptr[0]->type == CHAR) {
				semantic_error(T->pos, T->ptr[0]->type_id, "是字符类型变量，不能参与运算");
				break;
			}
			else if (T->ptr[1]->type == CHAR) {
				semantic_error(T->pos, T->ptr[1]->type_id, "是字符类型变量，不能参与运算");
				break;
			}
			if (T->ptr[0]->type == FLOAT && T->ptr[1]->type == FLOAT)
				T->type = FLOAT, T->width = T->ptr[0]->width + T->ptr[1]->width + 4;
			else if (T->ptr[0]->type == INT && T->ptr[1]->type == INT)
				T->type = INT, T->width = T->ptr[0]->width + T->ptr[1]->width + 2;
			else {
				semantic_error(T->pos, "", "运算符左右变量类型不匹配");
				break;
			}
			T->place = fill_Temp(newTemp(), LEV, T->type, 'T', T->offset + T->ptr[0]->width + T->ptr[1]->width);
			opn1.kind = ID;
			strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
			opn1.type = T->ptr[0]->type;
			opn1.offset = symbolTable.symbols[T->ptr[0]->place].offset;
			opn2.kind = ID;
			strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias);
			opn2.type = T->ptr[1]->type;
			opn2.offset = symbolTable.symbols[T->ptr[1]->place].offset;
			result.kind = ID;
			strcpy(result.id, symbolTable.symbols[T->place].alias);
			result.type = T->type;
			result.offset = symbolTable.symbols[T->place].offset;
			T->width = T->ptr[0]->width + T->ptr[1]->width + (T->type == INT ? 4 : 8);
			break;
		case NOT:   //未写完整
			T->type = INT;
			T->ptr[0]->offset = T->offset;
			Exp(T->ptr[0]);
			break;
		case UMINUS:
			T->type = T->ptr[0]->type;
			T->ptr[0]->offset = T->offset;
			Exp(T->ptr[0]);
			break;
		case DPLUS:
			rtn = searchSymbolTable(T->ptr[0]->type_id);
			if (T->ptr[0]->kind != ID && T->ptr[0]->kind != ARRAY_DEC && T->ptr[0]->kind != ARRAY_CALL && T->ptr[0]->kind != STRUCT_VISIT) {
				semantic_error(T->pos, "", "自增语句需要左值");
				break;
			}
			else if (rtn!=-1&&symbolTable.symbols[rtn].type==STRUCT)
			{
				semantic_error(T->pos, "", "结构体不能自增");
				break;
			}
			else {
				T->ptr[0]->offset = T->offset;
				Exp(T->ptr[0]);
				T->type = T->ptr[0]->type;
				T->width = T->ptr[0]->width;
				T->place = T->ptr[0]->place;
				opn1.kind = INT;
				opn1.const_int = 1;
				result.kind = ID;
				strcpy(result.id, newTemp());
				opn1.kind = ID;
				strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
			}
			break;
		case DMINUS:
			rtn = searchSymbolTable(T->ptr[0]->type_id);
			if (T->ptr[0]->kind != ID && T->ptr[0]->kind != ARRAY_DEC && T->ptr[0]->kind != ARRAY_CALL && T->ptr[0]->kind != STRUCT_VISIT) {
				semantic_error(T->pos, "", "自减语句需要左值");
				break;
			}
			else if (rtn != -1 && symbolTable.symbols[rtn].type == STRUCT)
			{
				semantic_error(T->pos, "", "结构体不能自减");
				break;
			}
			else {
				T->ptr[0]->offset = T->offset;
				Exp(T->ptr[0]);
				T->type = T->ptr[0]->type;
				T->width = T->ptr[0]->width;
				T->place = T->ptr[0]->place;
				opn1.kind = INT;
				opn1.const_int = 1;
				result.kind = ID;
				strcpy(result.id, newTemp());
				opn1.kind = ID;
				strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
			}
			break;
		case PLUSASSIGNOP:
		case MINUSASSIGNOP:
		case STARASSIGNOP:
		case DIVASSIGNOP:
		case MODASSIGNOP:
			if (T->ptr[0]->kind != ID && T->ptr[0]->kind != ARRAY_DEC && T->ptr[0]->kind != ARRAY_CALL&& T->ptr[0]->kind != STRUCT_VISIT) {
				semantic_error(T->pos, "", "复合赋值语句需要左值");
				break;
			}
			if (T->ptr[0]->type == CHAR) {
				semantic_error(T->pos, T->ptr[0]->type_id, "是字符类型变量，不能参与复合赋值运算");
				break;
			}
			if (T->ptr[1]->type == CHAR) {
				semantic_error(T->pos, "", "字符类型变量不能参与复合赋值运算");
				break;
			}
			T->ptr[0]->offset = T->offset;
			Exp(T->ptr[0]);
			T->ptr[1]->offset = T->offset + T->ptr[0]->width;
			Exp(T->ptr[1]);
			if (T->ptr[0]->type == FLOAT && T->ptr[1]->type == FLOAT) {
				T->type = FLOAT;
			}
			else if (T->ptr[0]->type == INT && T->ptr[1]->type == INT) {
				T->type = INT;
			}
			else {
				semantic_error(T->pos, "", "复合赋值运算左右变量类型不匹配");
			}
			break;
		case FUNC_CALL:
			rtn = searchSymbolTable(T->type_id);
			if (rtn == -1) {
				semantic_error(T->pos, T->type_id, "函数未定义");
				break;
			}
			if (symbolTable.symbols[rtn].flag != 'F') {
				semantic_error(T->pos, T->type_id, "不是函数名，不能调用函数");
				break;
			}
			T->type = symbolTable.symbols[rtn].type;
			if (T->type == INT) {
				width = 4;
			}
			else if (T->type == FLOAT) {
				width = 8;
			}
			else if (T->type == CHAR) {
				width = 1;
			}
			if (T->ptr[0]) {
				T->ptr[0]->offset = T->offset;
				Exp(T->ptr[0]);       //处理所有实参表达式求值，及类型
				T->width = T->ptr[0]->width + width; //累加上计算实参使用临时变量的单元数
				T->code = T->ptr[0]->code;
			}
			else if (symbolTable.symbols[rtn].paramnum != 0) {
				T->width = width;
				T->code = NULL;
				//semantic_error(T->pos, T->type_id, "该函数需要参数");
			}
			match_param(rtn, T);   //处理所有参数的匹配
			//处理参数列表的中间代码
			T0 = T->ptr[0];
			while (T0) {
				result.kind = ID;
				strcpy(result.id, symbolTable.symbols[T0->ptr[0]->place].alias);
				result.offset = symbolTable.symbols[T0->ptr[0]->place].offset;
				T0 = T0->ptr[1];
			}
			T->place = fill_Temp(newTemp(), LEV, T->type, 'T', T->offset + T->width - width);
			opn1.kind = ID;
			strcpy(opn1.id, T->type_id);  //保存函数名
			opn1.offset = rtn;  //这里offset用以保存函数定义入口,在目标代码生成时，能获取相应信息
			result.kind = ID;
			strcpy(result.id, symbolTable.symbols[T->place].alias);
			result.offset = symbolTable.symbols[T->place].offset;
			break;
		case ARRAY_CALL:
			rtn = searchSymbolTable(T->type_id);
			if (rtn == -1) {
				semantic_error(T->pos, T->type_id, "数组未定义");
				break;
			}
			else if (symbolTable.symbols[rtn].flag == 'F') {
				semantic_error(T->pos, T->type_id, "是函数名，不能采用下标访问");
				break;
			}
			else if (symbolTable.symbols[rtn].flag == 'V') {
				semantic_error(T->pos, T->type_id, "不是数组变量名，不能采用下标访问");
				break;
			}
			T->type = symbolTable.symbols[rtn].type;
			Exp(T->ptr[0]);
			callArray(T->ptr[0]);
			break;
		case STRUCT_VISIT:
			Exp(T->ptr[0]);
			if (symbolTable.symbols[T->ptr[0]->place].type != STRUCT) {
				semantic_error(T->pos, symbolTable.symbols[T->ptr[0]->place].name, "变量不是结构");
			}
			rtn = searchSymbolTable(symbolTable.symbols[T->ptr[0]->place].struct_name);
			rtn2 = searchSymbolTable(symbolTable.symbols[T->ptr[0]->place].name);
			if (rtn == -1) {
				semantic_error(T->pos, symbolTable.symbols[T->ptr[0]->place].struct_name, "结构为定义");
				return;
			}
			T->place = rtn2;
			do {
				rtn++;
				if (!strcmp(symbolTable.symbols[rtn].name, T->type_id)) {
					flag = 1;
					break;
				}
			} while (rtn < symbolTable.index && symbolTable.symbols[rtn].flag == 'M');

			if (!flag) {
				semantic_error(T->pos, T->type_id, "不是该结构的成员变量");
			}
			else {
				T->type = symbolTable.symbols[rtn].type;
				T->offset = symbolTable.symbols[rtn].offset;
			}
			break;
		case ARGS:      //此处仅处理各实参表达式的求值的代码序列，不生成ARG的实参系列
			T->ptr[0]->offset = T->offset;
			Exp(T->ptr[0]);
			T->width = T->ptr[0]->width;
			T->code = T->ptr[0]->code;
			if (T->ptr[1]) {
				T->ptr[1]->offset = T->offset + T->ptr[0]->width;
				Exp(T->ptr[1]);
				T->width += T->ptr[1]->width;
			}
			break;
		}
	}
}

void semantic_Analysis(struct ASTNode* T)
{//对抽象语法树的先根遍历,按display的控制结构修改完成符号表管理和语义检查和TAC生成（语句部分）
	int rtn, num, width;
	struct ASTNode* T0;
	struct opn opn1, opn2, result;
//	printf("%d\n", T->kind);
	if (T)
	{
//		printf("semantic_Analysis %d\n", T->offset);
		switch (T->kind) {
		case EXT_DEF_LIST:
			if (!T->ptr[0])
				break;
			// 语义分析之前设置偏移地址
			T->ptr[0]->offset = T->offset;
			semantic_Analysis(T->ptr[0]);    //访问外部定义列表中的第一个
			T->code = T->ptr[0]->code;

			if (T->ptr[1]) {
				T->ptr[1]->offset = T->ptr[0]->offset + T->ptr[0]->width;
				semantic_Analysis(T->ptr[1]); //访问该外部定义列表中的其它外部定义
			}
			break;
		case EXT_VAR_DEF:
			//处理外部说明,将第一个孩子(TYPE结点)中的类型送到第二个孩子的类型域
			if (!strcmp(T->ptr[0]->type_id, "int")) {
				T->type = T->ptr[1]->type = INT;
				T->ptr[1]->width = 4;
			}
			else if (!strcmp(T->ptr[0]->type_id, "float")) {
				T->type = T->ptr[1]->type = FLOAT;
				T->ptr[1]->width = 8;
			}
			else if (!strcmp(T->ptr[0]->type_id, "char")) {
				T->type = T->ptr[1]->type = CHAR;
				T->ptr[1]->width = 1;
			}
			else
			{
				T->type = T->ptr[1]->type = STRUCT;
				strcpy(struct_name, T->ptr[0]->ptr[0]->type_id);
				struct_var_flag = 1;
			}
			T->ptr[1]->offset = T->offset;        //这个外部变量的偏移量向下传递
			ext_var_list(T->ptr[1]);            //处理外部变量说明中的标识符序列
			T->width = (T->ptr[1]->width) * T->ptr[1]->num;//计算这个外部变量说明的宽度
			T->code = NULL;             //这里假定外部变量不支持初始化
			break;
		case EXT_STRUCT_DEC:
			T->ptr[0]->offset = T->offset;
			semantic_Analysis(T->ptr[0]);
			break;
		case STRUCT_NEW:
			T->width = 0;   //函数的宽度设置为0，不会对外部变量的地址分配产生影响
			T->offset = DX; //设置局部变量在活动记录中的偏移量初值
			T->type = STRUCT;
//			printf("%s\n", T->ptr[0]->type_id);
			rtn = fillSymbolTable(T->ptr[0]->type_id, newAlias(), LEV, STRUCT, 'S', 0); //函数不在数据区中分配单元，偏移量为0
			if (rtn == -1)
			{
				semantic_error(T->pos, T->ptr[0]->type_id, "结构体重复定义");
				return;
			}
			else
				T->place = rtn;
			T->ptr[1]->offset = T->offset;
			struct_flag = 1;
			semantic_Analysis(T->ptr[1]);
			struct_flag = 0;
			break;
		case STRUCT_DEC:
			T->ptr[0]->offset = T->offset;
			semantic_Analysis(T->ptr[0]);
			T->width = T->ptr[1]->width;
			break;
		case FUNC_DEF:      //填写函数定义信息到符号表
			if (!strcmp(T->ptr[0]->type_id, "int")) {
				T->ptr[1]->type = INT;
			}
			if (!strcmp(T->ptr[0]->type_id, "float")) {
				T->ptr[1]->type = FLOAT;
			}
			if (!strcmp(T->ptr[0]->type_id, "char")) {
				T->ptr[1]->type = CHAR;
			}
			if (!strcmp(T->ptr[0]->type_id, "struct"))
			{
				T->ptr[1]->type = STRUCT;
			}
			T->width = 0;     //函数的宽度设置为0，不会对外部变量的地址分配产生影响
			T->offset = DX;   //设置局部变量在活动记录中的偏移量初值
			semantic_Analysis(T->ptr[1]); //处理函数名和参数结点部分，这里不考虑用寄存器传递参数
			T->offset += T->ptr[1]->width;   //用形参单元宽度修改函数局部变量的起始偏移量
			T->ptr[2]->offset = T->offset;
			strcpy(T->ptr[2]->Snext, newLabel());  //函数体语句执行结束后的位置属性
			return_flag = 0;
			return_flag++;
			semantic_Analysis(T->ptr[2]);         //处理函数体结点
			if (return_flag >= 1)
				printf("%s 函数没有return语句\n", T->ptr[1]->type_id);
			//计算活动记录大小,这里offset属性存放的是活动记录大小，不是偏移
			symbolTable.symbols[T->ptr[1]->place].offset = T->offset + T->ptr[2]->width;
			break;
		case FUNC_DEC:      //根据返回类型，函数名填写符号表
			rtn = fillSymbolTable(T->type_id, newAlias(), LEV, T->type, 'F', 0);//函数不在数据区中分配单元，偏移量为0
			if (rtn == -1) {
				semantic_error(T->pos, T->type_id, "函数重复定义");
				break;
			}
			else
				T->place = rtn;
			result.kind = ID;
			strcpy(result.id, T->type_id);
			result.offset = rtn;
			T->offset = DX;   //设置形式参数在活动记录中的偏移量初值
			if (T->ptr[0]) { //判断是否有参数
				T->ptr[0]->offset = T->offset;
				semantic_Analysis(T->ptr[0]);  //处理函数参数列表
				T->width = T->ptr[0]->width;
				symbolTable.symbols[rtn].paramnum = T->ptr[0]->num;
			}
			else
				symbolTable.symbols[rtn].paramnum = 0, T->width = 0;
			break;
		case PARAM_LIST:    //处理函数形式参数列表
			T->ptr[0]->offset = T->offset;
			semantic_Analysis(T->ptr[0]);
			if (T->ptr[1]) {
				T->ptr[1]->offset = T->offset + T->ptr[0]->width;
				semantic_Analysis(T->ptr[1]);
				T->num = T->ptr[0]->num + T->ptr[1]->num;        //统计参数个数
				T->width = T->ptr[0]->width + T->ptr[1]->width;  //累加参数单元宽度
			}
			else {
				T->num = T->ptr[0]->num;
				T->width = T->ptr[0]->width;
				T->code = T->ptr[0]->code;
			}
			break;
		case  PARAM_DEC:
			rtn = fillSymbolTable(T->ptr[1]->type_id, newAlias(), 1, T->ptr[0]->type, 'P', T->offset);
			if (rtn == -1)
				semantic_error(T->ptr[1]->pos, T->ptr[1]->type_id, "参数名重复定义");
			else
				T->ptr[1]->place = rtn;
			T->num = 1;       //参数个数计算的初始值
			T->width = T->ptr[0]->type == INT ? 4 : 8;  //参数宽度
			result.kind = ID;
			strcpy(result.id, symbolTable.symbols[rtn].alias);
			result.offset = T->offset;
			break;
		case COMP_STM:
			LEV++;
			//设置层号加1，并且保存该层局部变量在符号表中的起始位置在symbol_scope_TX
			symbol_scope_TX.TX[symbol_scope_TX.top++] = symbolTable.index;
			T->width = 0;
			T->code = NULL;
			if (T->ptr[0]) {
				T->ptr[0]->offset = T->offset;
				semantic_Analysis(T->ptr[0]);  //处理该层的局部变量DEF_LIST
				T->width += T->ptr[0]->width;
				T->code = T->ptr[0]->code;
			}
			if (T->ptr[1]) {
				T->ptr[1]->offset = T->offset + T->width;
				strcpy(T->ptr[1]->Snext, T->Snext);  //S.next属性向下传递
				semantic_Analysis(T->ptr[1]);       //处理复合语句的语句序列
				T->width += T->ptr[1]->width;
			}
			prn_symbol();       //c在退出一个符合语句前显示的符号表
			LEV--;    //出复合语句，层号减1
			symbolTable.index = symbol_scope_TX.TX[--symbol_scope_TX.top]; //删除该作用域中的符号
			break;
		case DEF_LIST:
			T->code = NULL;
			if (T->ptr[0]) {
				T->ptr[0]->offset = T->offset;
				semantic_Analysis(T->ptr[0]);   //处理一个局部变量定义
				T->code = T->ptr[0]->code;
				T->width = T->ptr[0]->width;
			}
//			printf("DEF_LIST %d\n", T->ptr[0]->width);
			if (T->ptr[1]) {
				T->ptr[1]->offset = T->offset + T->ptr[0]->width;
				semantic_Analysis(T->ptr[1]);   //处理剩下的局部变量定义
				T->width += T->ptr[1]->width;
			}
			break;
		case VAR_DEF:
			//处理一个局部变量定义,将第一个孩子(TYPE结点)中的类型送到第二个孩子的类型域
		 //类似于上面的外部变量EXT_VAR_DEF，换了一种处理方法
//			printf("VAR_DEF %d\n", T->offset);
			T->code = NULL;
			if (!strcmp(T->ptr[0]->type_id, "int")) {
				T->ptr[1]->type = INT;
				width = 4;
			}
			else if (!strcmp(T->ptr[0]->type_id, "float")) {
				T->ptr[1]->type = FLOAT;
				width = 8;
			}
			else if (!strcmp(T->ptr[0]->type_id, "char")) {
				T->ptr[1]->type = CHAR;
				width = 1;
			}
			else
			{
				T->ptr[1]->type = STRUCT;
				strcpy(struct_name, T->ptr[0]->ptr[0]->type_id);
				struct_var_flag = 1;
				width = 4;
			}
			T0 = T->ptr[1]; //T0为变量名列表子树根指针，对ID、ASSIGNOP类结点在登记到符号表，作为局部变量
			num = 0;
			T0->offset = T->offset;
			T->width = 0;
			while (T0) {
				//处理所有DEC_LIST结点
//				printf("%d\n", T0->ptr[0]->kind);
				num++;
				T0->ptr[0]->type = T0->type;  //类型属性向下传递
				if (T0->ptr[1])
					T0->ptr[1]->type = T0->type;
				T0->ptr[0]->offset = T0->offset;  //类型属性向下传递
				if (T0->ptr[1])
					T0->ptr[1]->offset = T0->offset + width;
				if (T0->ptr[0]->kind == ID)
				{
					if (!struct_flag)
						rtn = fillSymbolTable(T0->ptr[0]->type_id, newAlias(), LEV, T0->ptr[0]->type, 'V', T->offset + T->width); //此处偏移量未计算，暂时为0
					else {
						rtn = fillSymbolTable(T0->ptr[0]->type_id, newAlias(), LEV + 1, T0->ptr[0]->type, 'M', T->offset + T->width); //此处偏移量未计算，暂时为0
					}
					if (rtn == -1)
						semantic_error(T0->ptr[0]->pos, T0->ptr[0]->type_id, "变量重复定义");
					else
						T0->ptr[0]->place = rtn;
					T->width += width;
				}
				else if (T0->ptr[0]->kind == ASSIGNOP) {
					rtn = fillSymbolTable(T0->ptr[0]->ptr[0]->type_id, newAlias(), LEV, T0->ptr[0]->type, 'V', T->offset + T->width);//此处偏移量未计算，暂时为0
					if (rtn == -1)
						semantic_error(T0->ptr[0]->ptr[0]->pos, T0->ptr[0]->ptr[0]->type_id, "变量重复定义");
					else {
						T0->ptr[0]->place = rtn;
						T0->ptr[0]->ptr[1]->offset = T->offset + T->width + width;
						Exp(T0->ptr[0]->ptr[1]);
						opn1.kind = ID;
						strcpy(opn1.id, symbolTable.symbols[T0->ptr[0]->ptr[1]->place].alias);
						result.kind = ID;
						strcpy(result.id, symbolTable.symbols[T0->ptr[0]->place].alias);
					}
					T->width += width + T0->ptr[0]->ptr[1]->width;
				}
				else if (T0->ptr[0]->kind == ARRAY_DF)
				{
					rtn = fillSymbolTable(T0->ptr[0]->type_id, newAlias(), LEV, T0->ptr[0]->type, 'A', T->offset + T->width);
					if (rtn == -1)
						semantic_error(T0->ptr[0]->pos, T0->ptr[0]->type_id, "变量重复定义");
					else
					{
						T0->ptr[0]->place = rtn;
						if (fillArray(T0->ptr[0]->ptr[0], symbolTable.symbols[T0->ptr[0]->place].array, 0) == -1)
						{
							semantic_error(T0->ptr[0]->pos, T0->ptr[0]->type_id, "数组维度过大");
							break;
						}
					}
					T->width += width * calArrayNums(T0->ptr[0]->ptr[0]);
				}
				T0 = T0->ptr[1];
			}
			break;
		case STM_LIST:
			if (!T->ptr[0]) {
				T->code = NULL;
				T->width = 0;
				break;
			}   //空语句序列
			if (T->ptr[1]) //2条以上语句连接，生成新标号作为第一条语句结束后到达的位置
				strcpy(T->ptr[0]->Snext, newLabel());
			else     //语句序列仅有一条语句，S.next属性向下传递
				strcpy(T->ptr[0]->Snext, T->Snext);
			T->ptr[0]->offset = T->offset;
			semantic_Analysis(T->ptr[0]);
			T->code = T->ptr[0]->code;
			T->width = T->ptr[0]->width;
			if (T->ptr[1]) {     //2条以上语句连接,S.next属性向下传递
				strcpy(T->ptr[1]->Snext, T->Snext);
				T->ptr[1]->offset = T->offset;  //顺序结构共享单元方式
//                  T->ptr[1]->offset=T->offset+T->ptr[0]->width; //顺序结构顺序分配单元方式
				semantic_Analysis(T->ptr[1]);
				//序列中第1条为表达式语句，返回语句，复合语句时，第2条前不需要标号
				if (T->ptr[1]->width > T->width) T->width = T->ptr[1]->width; //顺序结构共享单元方式
//                        T->width+=T->ptr[1]->width;//顺序结构顺序分配单元方式
			}
			break;
		case IF_THEN:
			strcpy(T->ptr[0]->Etrue, newLabel());  //设置条件语句真假转移位置
			strcpy(T->ptr[0]->Efalse, T->Snext);
			T->ptr[0]->offset = T->ptr[1]->offset = T->offset;
			boolExp(T->ptr[0]);
			T->width = T->ptr[0]->width;
			strcpy(T->ptr[1]->Snext, T->Snext);
			semantic_Analysis(T->ptr[1]);      //if子句
			if (T->width < T->ptr[1]->width)
				T->width = T->ptr[1]->width;
			break;  //控制语句都还没有处理offset和width属性
		case IF_THEN_ELSE:
			strcpy(T->ptr[0]->Etrue, newLabel());  //设置条件语句真假转移位置
			strcpy(T->ptr[0]->Efalse, newLabel());
			T->ptr[0]->offset = T->ptr[1]->offset = T->ptr[2]->offset = T->offset;
			boolExp(T->ptr[0]);      //条件，要单独按短路代码处理
			T->width = T->ptr[0]->width;
			strcpy(T->ptr[1]->Snext, T->Snext);
			semantic_Analysis(T->ptr[1]);      //if子句
			if (T->width < T->ptr[1]->width)
				T->width = T->ptr[1]->width;
			strcpy(T->ptr[2]->Snext, T->Snext);
			semantic_Analysis(T->ptr[2]);      //else子句
			if (T->width < T->ptr[2]->width)
				T->width = T->ptr[2]->width;
			break;
		case WHILE:
			T->ptr[0]->offset = T->ptr[1]->offset = T->offset;
			boolExp(T->ptr[0]);      //循环条件，要单独按短路代码处理
			T->width = T->ptr[0]->width;
			strcpy(T->ptr[1]->Snext, newLabel());

			flag++;  //修改成+1来控制层数

			semantic_Analysis(T->ptr[1]);      //循环体
			if (T->width < T->ptr[1]->width)
				T->width = T->ptr[1]->width;
			flag--;
			break;
		case FOR:
			LEV++;
			T->ptr[0]->offset = T->offset;
			symbol_scope_TX.TX[symbol_scope_TX.top++] = symbolTable.index;
			semantic_Analysis(T->ptr[0]);
			T->ptr[2]->offset = T->offset + T->ptr[0]->width;
			semantic_Analysis(T->ptr[2]);
			T->ptr[1]->offset = T->ptr[2]->offset + T->ptr[2]->width;
			boolExp(T->ptr[1]);
			T->width = T->ptr[0]->width + T->ptr[1]->width + T->ptr[2]->width;
			T->ptr[3]->offset = T->ptr[2]->offset + T->ptr[2]->width;
			flag++;
			//semantic_Analysis(T->ptr[1]);
			semantic_Analysis(T->ptr[3]);
			if (T->width < T->ptr[3]->width)
				T->width = T->ptr[3]->width;
			flag--;
			LEV--;
			break;
		case EXP_STMT:
			T->ptr[0]->offset = T->offset;
			semantic_Analysis(T->ptr[0]);
			T->code = T->ptr[0]->code;
			T->width = T->ptr[0]->width;
			break;
		case RETURN:
			return_flag--;
			if (T->ptr[0]) {
				T->ptr[0]->offset = T->offset;
				Exp(T->ptr[0]);
				num = symbolTable.index;
				do
					num--;
				while (symbolTable.symbols[num].flag != 'F');
				if (T->ptr[0]->type != symbolTable.symbols[num].type) {
					semantic_error(T->pos, "返回值类型错误，语义错误", "");
					T->width = 0;
					T->code = NULL;
					return;
				}
				/*需要判断返回值类型是否匹配*/

				T->width = T->ptr[0]->width;
				result.kind = ID;
				strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
				result.offset = symbolTable.symbols[T->ptr[0]->place].offset;
			}
			else {
				T->width = 0;
				result.kind = 0;
			}
			break;
		case SWITCH_STMT:
			flag++;
			T->ptr[0]->offset = T->offset;
			Exp(T->ptr[0]);
			T->width = T->ptr[0]->width;
			// case语句
			T->ptr[1]->offset = T->offset + T->width;
			strcpy(case_temp, symbolTable.symbols[T->ptr[0]->place].name);
			symbolTable.symbols[T->ptr[0]->place].type = INT;
			semantic_Analysis(T->ptr[1]);
			flag--;
			break;
		case CASE_STMT:
			left = (struct ASTNode*)malloc(sizeof(struct ASTNode));
			right = (struct ASTNode*)malloc(sizeof(struct ASTNode));
			left->kind = ID;
			strcpy(left->type_id, case_temp);
			if (T->ptr[0]->type == INT) {
				right->kind = INT;
				right->type = INT;
				right->type_int = T->ptr[0]->type_int;
			}
			T->ptr[0]->ptr[0] = left;
			T->ptr[0]->ptr[1] = right;
			T->ptr[0]->kind = RELOP;
			strcpy(T->ptr[0]->type_id, "==");
			T->ptr[0]->offset = T->ptr[1]->offset = T->offset;
			boolExp(T->ptr[0]);
			T->width = T->ptr[0]->width;
			semantic_Analysis(T->ptr[1]); //if子句
			if (T->width < T->ptr[1]->width)
				T->width = T->ptr[1]->width;
			break;
		case BREAK:
			if (flag == 0)
				semantic_error(T->pos, " ", "break语句出现位置错误");
			break;
		case CONTINUE:
			if (flag == 0)
				semantic_error(T->pos, " ", "continue语句出现位置错误");
			break;
		case ID:
		case INT:
		case FLOAT:
		case CHAR:
		case ARRAY_DEC:
		case ASSIGNOP:
		case AND:
		case OR:
		case RELOP:
		case PLUS:
		case MINUS:
		case STAR:
		case DIV:
		case PLUSASSIGNOP:
		case MINUSASSIGNOP:
		case STARASSIGNOP:
		case DIVASSIGNOP:
		case MODASSIGNOP:
		case STRUCT_VISIT:
		case DPLUS:
		case DMINUS:
		case NOT:
		case UMINUS:
		case FUNC_CALL:
		case ARRAY_CALL:
		case ARGS:
			Exp(T);          //处理基本表达式
			break;
		}
	}
}

void semantic_Analysis0(struct ASTNode* T) {
	symbolTable.index = 0;
	fillSymbolTable("read", "", 0, INT, 'F', 4);
	symbolTable.symbols[0].paramnum = 0;//read的形参个数
	fillSymbolTable("write", "", 0, INT, 'F', 4);
	symbolTable.symbols[1].paramnum = 1;
	fillSymbolTable("x", "", 1, INT, 'P', 12);
	symbol_scope_TX.TX[0] = 0;  //外部变量在符号表中的起始序号为0
	symbol_scope_TX.top = 1;
	T->offset = 0;              //外部变量在数据区的偏移量
	semantic_Analysis(T);
}
