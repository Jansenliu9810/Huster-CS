/* Linear Table On Sequence Structure */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

/*---------page 10 on textbook ---------*/
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2

typedef int status;
typedef int ElemType; //数据元素类型定义

/*-------page 22 on textbook -------*/
#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
typedef struct {  //顺序表（顺序结构）的定义
	ElemType* elem;
	int length;
	int listsize;
}SqList;
/*-----page 19 on textbook ---------*/
status InitList(SqList& L)
// 线性表L不存在，构造一个空的线性表，返回OK，否则返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/

	L.elem = (ElemType*)malloc(LIST_INIT_SIZE * sizeof(ElemType));
	if (!L.elem)
		return INFEASTABLE;

	L.length = 0;
	L.listsize = LIST_INIT_SIZE;
	return OK;
	/********** End **********/
}

status DestroyList(SqList& L)
// 如果线性表L存在，销毁线性表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L.elem) {
		L.length = 0;
		L.listsize = 0;
		free(L.elem);
		L.elem = NULL;
		return OK;
	}
	else
	{
		return INFEASTABLE;
	}

	/********** End **********/
}

status ClearList(SqList& L)
// 如果线性表L存在，删除线性表L中的所有元素，返回OK，否则返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L.elem)
	{
		L.length = 0;
		return OK;
	}
	return INFEASTABLE;
	/********** End **********/
}

status ListEmpty(SqList L)
// 如果线性表L存在，判断线性表L是否为空，空就返回TRUE，否则返回FALSE；如果线性表L不存在，返回INFEASTABLE。
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    if(L.elem)
    {
        if(L.length == 0)
        {
            return TRUE;
        }
        return FALSE;
    }
    return INFEASTABLE;
    /********** End **********/
}

status ListLength(SqList L)
// 如果线性表L存在，返回线性表L的长度，否则返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L.elem)
	{
		return L.length;
	}
	return INFEASTABLE;
	/********** End **********/
}

status GetElem(SqList L, int i, ElemType& e)
// 如果线性表L存在，获取线性表L的第i个元素，保存在e中，返回OK；如果i不合法，返回ERROR；如果线性表L不存在，返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L.elem)
	{
		if (i<1 || i>L.length)
		{
			return ERROR;
		}
		e = L.elem[i];
		return OK;
	}
	return INFEASTABLE;
	/********** End **********/
}

status LocateElem(SqList L, ElemType e)
// 如果线性表L存在，查找元素e在线性表L中的位置序号并返回OK；如果e不存在，返回ERROR；当线性表L不存在时，返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L.elem)
	{
		int i = 1;
		while (i < L.length + 1 && L.elem[i] != e)
			i += 1;
		if (i >= L.length+1)
		{
			return ERROR;
		}
		return i;
		return OK;
	}
	return INFEASTABLE;
	/********** End **********/
}

status PriorElem(SqList L, ElemType e, ElemType& pre)
// 如果线性表L存在，获取线性表L中元素e的前驱，保存在pre中，返回OK；如果没有前驱，返回ERROR；如果线性表L不存在，返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L.elem)
	{
		int i = 1;
		while (i < L.length && e != L.elem[i])
		{
			i++;
		}
		if (e == L.elem[1] || i >= L.length+1)
			return ERROR;
		pre = L.elem[i - 1];
		return OK;
	}
	return INFEASTABLE;
	/********** End **********/
}

status NextElem(SqList L, ElemType e, ElemType& next)
// 如果线性表L存在，获取线性表L元素e的后继，保存在next中，返回OK；如果没有后继，返回ERROR；如果线性表L不存在，返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L.elem)
	{
		int i = LocateElem(L,e);
		if (i != 0 && i != L.length)
		{
			next = L.elem[i + 1];
			return OK;
		}
		return ERROR;
	}
	return INFEASTABLE;
	/********** End **********/
}

status ListInsert(SqList& L, int i, ElemType e)
// 如果线性表L存在，将元素e插入到线性表L的第i个元素之前，返回OK；当插入位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L.elem)
	{
		int a;
		if (i<1 || i>L.length + 1)
			return ERROR;
		if (L.length >= L.listsize)
		{
			ElemType* newbase;
			newbase = (ElemType*)realloc(L.elem, (L.listsize + LISTINCREMENT) * sizeof(ElemType));
			if (!newbase)
				return OVERFLOW;
			L.elem = newbase;
			L.listsize = LISTINCREMENT;
		}
		for (a = L.length; a >= i - 1; a--)
			L.elem[a + 1] = L.elem[a];
		L.elem[i] = e;
		L.length++;
		return OK;
	}
	return INFEASTABLE;
	/********** End **********/
}

status ListDelete(SqList& L, int i, ElemType& e)
// 如果线性表L存在，删除线性表L的第i个元素，并保存在e中，返回OK；当删除位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L.elem)
	{
		if (i<1 || i>L.length)
			return ERROR;
		int* p = &(L.elem[i]);
		e = *p;
		int *q = L.elem + L.length;
		for (++p; *p <= *q; ++p)
			*(p - 1) = *p;
		L.length--;
		return OK;
	}
	return INFEASTABLE;
	/********** End **********/
}

status ListTraverse(SqList L)
// 如果线性表L存在，依次显示线性表中的元素，每个元素间空一格，返回OK；如果线性表L不存在，返回INFEASIBLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L.elem)
	{
		int i;
		for (i = 1; i < L.length+1; i++)
		{
			if (i == L.length+1)
				printf("%d", L.elem[i]);
			else
				printf("%d ", L.elem[i]);
		}
		return OK;
	}
	return INFEASTABLE;
	/********** End **********/
}
/*--------------------------------------------*/
void main(void) {
	SqList L;  int op = 1;
	int u = 1;
	int e, i, pre_e, next_e, current_e, elem;
	printf("请输入对哪一个线性表进行操作1-99，若输入0则退出程序！\n");
	scanf("%d", &u);
	if (u == 0)
		return;
	while (op) {
		system("cls");	printf("\n\n");
		printf("      Menu for Linear Table On Sequence Structure \n");
		printf("-------------------------------------------------\n");
		printf("    	  1. InitList       7. LocateElem\n");
		printf("    	  2. DestroyList    8. PriorElem\n");
		printf("    	  3. ClearList      9. NextElem \n");
		printf("    	  4. ListEmpty      10. ListInsert\n");
		printf("    	  5. ListLength     11. ListDelete\n");
		printf("    	  6. GetElem        12. ListTrabverse\n");
		printf("    	  0. Exit\n");
		printf("-------------------------------------------------\n");
		printf("      请选择你的操作[0~12]:");
		scanf("%d", &op);
		switch (op) {
			case 1:
				//printf("\n----IntiList功能待实现！\n");
				if (InitList(L) == OK)
					printf("\t\t线性表创建成功！\n");
				else
				{
					printf("\t\t给线性表动态分配空间失败！\n");
					printf("\t\t线性表创建失败！\n");
				}
				getchar();
				printf("\n\t\t(按任意键继续..........)\n");
				getchar();
				break;
			case 2:
				if (L.elem)
				{
					if (DestroyList(L) == OK)
						printf("\t\t线性表删除成功！\n");
					else
						printf("\t\t线性表删除失败！\n");
				}
				else
				{
					printf("\t\t线性表不存在！\n");
				}
				getchar();
				printf("\n\t\t(按任意键继续..........)\n");
				getchar();
				break;
			case 3:
				if (L.elem)
				{
					if (ClearList(L) == OK)
						printf("\t\t线性表清空成功！\n");
					else
						printf("\t\t线性表清空失败！\n");
				}
				else
				{
					printf("\t\t线性表不存在！\n");
				}
				getchar();
				printf("\n\t\t(按任意键继续..........)\n");
				getchar();
				break;
			case 4:
				if (L.elem)
				{
					if (ListEmpty(L) == TRUE)
						printf("\t\t此线性表为空！\n");
					else
						printf("\t\t此线性表不为空！\n");
				}
				else
				{
					printf("\t\t线性表不存在！\n");
				}
				getchar();
				printf("\n\t\t(按任意键继续..........)\n");
				getchar();
				break;
			case 5:
				if(L.elem)
					printf("\t\t线性表的长度为：%d\t\n", ListLength(L));
				else
					printf("\t\t线性表不存在！\n");
				getchar();
				printf("\n\t\t(按任意键继续..........)\n");
				getchar();
				break;
			case 6:
				if (L.elem)
				{
					printf("\t\t请输入想要获得的数据元素的位序：");
					scanf("%d", &i);
					if (GetElem(L, i, e) == ERROR)
						printf("\n\t\t该数据元素不在范围内！\n");
					else
						printf("\n\t\t线性表L中第%d个数据元素的值是：%d\n", i, e);
				}
				else
				{
					printf("\t\t线性表不存在！\n");
				}
				getchar();
				printf("\n\t\t(按任意键继续..........)\n");
				getchar();
				break;
			case 7:
				if (L.elem)
				{
					printf("\t\t请输入需要查找的数据元素：");
					scanf("%d", &e);
					if (LocateElem(L, e) == ERROR)
						printf("\n\t\t数据元素的值查找失败！该元素不存在！\n");
					else
						printf("\n\t\t该数据元素值在线性表中为第%d个元素！\n", LocateElem(L, e));
				}
				else
				{
					printf("\t\t线性表不存在！\n");
				}
				getchar();
				printf("\n\t\t(按任意键继续..........)\n");
				getchar();
				break;
			case 8:
				if (L.elem)
				{
					printf("\t\t请输入一个数据元素用于查找此前驱结点元素：");
					scanf("%d", &current_e);
					if (PriorElem(L, current_e, pre_e) == ERROR)
						printf("\n\t\t该数据元素没有前驱结点元素！\n");
					else
						printf("\n\t\t该数据元素%d的前驱结点元素为：%d\n", current_e, pre_e);
				}
				else
				{
					printf("\t\t线性表不存在！\n");
				}
				getchar();
				printf("\n\t\t(按任意键继续..........)\n");
				getchar();
				break;
			case 9:
				if (L.elem)
				{
					printf("\t\t请输入一个数据元素用于查找此后继结点元素：");
					scanf("%d", &current_e);
					if (NextElem(L, current_e, next_e) == ERROR)
						printf("\n\t\t该数据元素没有后继结点元素！\n");
					else
						printf("\n\t\t该数据元素%d的后继结点元素为：%d\n", current_e, next_e);
				}
				else
				{
					printf("\t\t线性表不存在！\n");
				}
				getchar();
				printf("\n\t\t(按任意键继续..........)\n");
				getchar();
				break;
			case 10:
				if (L.elem)
				{
					printf("\t\t请输入插入数据元素的位置：");
					scanf("%d", &i);
					getchar();
					printf("\t\t请输入插入数据元素的值：");
					scanf("%d", &e);
					getchar();
					if (ListInsert(L, i, e) == ERROR)
						printf("\n\t\t新数据元素插入失败！\n");
					else
						printf("\n\t\t新数据元素插入成功！线性表的长度为：%d\n", L.length);
				}
				else
				{
					printf("\t\t线性表不存在！\n");
				}
				getchar();
				printf("\n\t\t(按任意键继续..........)\n");
				getchar();
				break;
			case 11:
				if (L.elem)
				{
					printf("\t\t请输入要删除的数据元素在线性表中的位序：");
					scanf("%d", &i);
					if (ListDelete(L, i, e) == ERROR)
						printf("\n\t\t数据元素删除失败！\n");
					else
						printf("\n\t\t数据元素删除成功！被删除的数据元素为%d,表的长度是%d !\n", e, L.length);
				}
				else
				{
					printf("\t\t线性表不存在！\n");
				}
				getchar();
				printf("\n\t\t(按任意键继续..........)\n");
				getchar();
				break;
			case 12:
				//printf("\n----ListTrabverse功能待实现！\n");
				if (ListTraverse(L))
				{
					if (L.length == 0)
						printf("\t\t线性表是空表！\n");
				}
				else
				{
					printf("\t\t线性表不存在！\n");
				}
				getchar();
				printf("\n\t\t(按任意键继续..........)\n");
				getchar();
				break;
			case 0:
				break;
		}//end of switch
	}//end of while
	printf("欢迎下次再使用本系统！\n");
}//end of main()
/*--------page 23 on textbook --------------------*/