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
typedef struct LNode{  //单链表（链式结构）的定义
	ElemType data;
	struct LNode* next;
}LNode,*LinkList;
/*-----page 19 on textbook ---------*/
status InitList(LinkList& L)
// 单链表L不存在，构造一个空的单链表，返回OK，否则返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/

	if (!L)
	{
		L = (LinkList)malloc(sizeof(LNode));
		if (!L)
		{
			return ERROR;
		}
		L->data = 0;
		L->next = 0;
		return OK;
	}
	return INFEASTABLE;
	/********** End **********/
}

status DestroyList(LinkList& L)
// 如果单链表L存在，销毁单链表L，释放数据元素的空间，返回OK，否则返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L)
	{
		LinkList p, q;
		p = L;
		while (p)
		{
			q = p->next;
			free(p);
			p = q;
		}
		L = NULL;
		return OK;
	}
	return INFEASTABLE;

	/********** End **********/
}

status ClearList(LinkList& L)
// 如果单链表L存在，删除单链表L中的所有元素，返回OK，否则返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L)
	{
		LinkList p, q;
		p = L->next;
		while (p)
		{
			q = p;
			p = p->next;
			free(q);
		}
		L->next = 0;
		return OK;
	}
	return INFEASTABLE;

	/********** End **********/
}

status ListEmpty(LinkList L)
// 如果单链表L存在，判断单链表L是否为空，空就返回TRUE，否则返回FALSE；如果单链表L不存在，返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L)
	{
		if (L->next == 0)
			return TRUE;
		return FALSE;
	}
	return INFEASTABLE;

	/********** End **********/
}

int ListLength(LinkList L)
// 如果单链表L存在，返回单链表L的长度，否则返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L)
	{
		LinkList p = L->next;
		int i = 0;
		while (p)
		{
			i++;
			p = p->next;
		}
		return i;
	}
	return INFEASTABLE;

	/********** End **********/
}

status GetElem(LinkList L, int i, ElemType& e)
// 如果单链表L存在，获取单链表L的第i个元素，保存在e中，返回OK；如果i不合法，返回ERROR；如果单链表L不存在，返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L)
	{
		LinkList p = L->next;
		int j = 1;
		while (p && j < i)
		{
			p = p->next;
			j++;
		}
		if (!p || j > i)
			return ERROR;
		e = p->data;
		return OK;
	}
	return INFEASTABLE;

	/********** End **********/
}

status LocateElem(LinkList L, ElemType e)
// 如果单链表L存在，查找元素e在单链表L中的位置序号；如果e不存在，返回ERROR；当单链表L不存在时，返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L)
	{
		int i = 1;
		LinkList p = L->next;
		while (p && p->data != e)
		{
			p = p->next;
			i++;
		}
		if (!p)
			return ERROR;
		return i;
	}
	return INFEASTABLE;

	/********** End **********/
}

status PriorElem(LinkList L, ElemType e, ElemType& pre)
// 如果单链表L存在，获取单链表L中元素e的前驱，保存在pre中，返回OK；如果没有前驱，返回ERROR；如果单链表L不存在，返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L)
	{
		LinkList p = L->next;
		LinkList q = L;
		if (e == 1)
			return ERROR;
		while (p && p->data != e)
		{
			p = p->next;
			q = q->next;
		}
		if (!p || q->data == 0)
			return ERROR;
		p = q;
		pre = q->data;
		return OK;
	}
	return INFEASTABLE;

	/********** End **********/
}

status NextElem(LinkList L, ElemType e, ElemType& next)
// 如果单链表L存在，获取单链表L元素e的后继，保存在next中，返回OK；如果没有后继，返回ERROR；如果单链表L不存在，返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L)
	{
		LinkList p = L->next;
		while (p && p->data != e)
		{
			p = p->next;
		}
		if (!p || p->next == 0)
			return ERROR;
		next = p->next->data;
		return OK;
	}
	return INFEASTABLE;
	/********** End **********/
}

status ListInsert(LinkList& L, int i, ElemType e)
// 如果单链表L存在，将元素e插入到单链表L的第i个元素之前，返回OK；当插入位置不正确时，返回ERROR；如果单链表L不存在，返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L)
	{
		LinkList p = L;
		int j = 0;
		while (p && j < i - 1)
		{
			p = p->next;
			++j;
		}
		if (!p || p == 0 || i <= 0)
			return ERROR;
		LinkList q = (LinkList)malloc(sizeof(LNode));
		q->data = e;
		q->next = p->next;
		p->next = q;
		return OK;
	}
	return INFEASTABLE;

	/********** End **********/
}

status ListDelete(LinkList& L, int i, ElemType& e)
// 如果单链表L存在，删除单链表L的第i个元素，并保存在e中，返回OK；当删除位置不正确时，返回ERROR；如果单链表L不存在，返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L)
	{
		LinkList p = L;
		int j = 0;
		while (p && j < i - 1)
		{
			p = p->next;
			j++;
		}
		if (!p || i - 1 < j || p == 0)
			return ERROR;
		LinkList q = p;
		e = q->next->data;
		q = p->next;
		p->next = q->next;
		free(q);
		return OK;
	}
	return INFEASTABLE;

	/********** End **********/
}

status ListTraverse(LinkList L)
// 如果单链表L存在，依次显示单链表中的元素，每个元素间空一格，返回OK；如果单链表L不存在，返回INFEASTABLE。
{
	// 请在这里补充代码，完成本关任务
	/********** Begin *********/
	if (L)
	{
		LinkList p = L->next;
		while (p)
		{
			printf(" %d", p->data);
			p = p->next;
		}
		printf("\n");
		return OK;
	}
	return INFEASTABLE;

	/********** End **********/
}
/*--------------------------------------------*/
void main(void) {
	LinkList L = 0;  int op = 1;
	int u = 1;
	int i;
	ElemType e, pre_e, next_e, current_e;
	printf("请输入对哪一个单链表进行操作1-99，若输入0则退出程序！\n");
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
			if (!L)
			{
				if (InitList(L) == OK)
					printf("\t\t单链表创建成功！\n");
				else
				{
					printf("\t\t单链表创建失败！\n");
				}
			}
			else
			{
				printf("\t\t单链表已存在，请清除原有单链表后再进行初始化单链表操作！\n");
			}
			getchar();
			printf("\n\t\t(按任意键继续..........)\n");
			getchar();
			break;
		case 2:
			if (L)
			{
				if (DestroyList(L) == OK)
					printf("\t\t单链表删除成功！\n");
				else
					printf("\t\t单链表删除失败！\n");
			}
			else
			{
				printf("\t\t单链表不存在！\n");
			}
			getchar();
			printf("\n\t\t(按任意键继续..........)\n");
			getchar();
			break;
		case 3:
			if (L)
			{
				if (ClearList(L) == OK)
					printf("\t\t单链表清空成功！\n");
				else
					printf("\t\t单链表清空失败！\n");
			}
			else
			{
				printf("\t\t单链表不存在！\n");
			}
			getchar();
			printf("\n\t\t(按任意键继续..........)\n");
			getchar();
			break;
		case 4:
			if (L)
			{
				if (ListEmpty(L) == TRUE)
					printf("\t\t此单链表为空！\n");
				else
					printf("\t\t此单链表不为空！\n");
			}
			else
			{
				printf("\t\t单链表不存在！\n");
			}
			getchar();
			printf("\n\t\t(按任意键继续..........)\n");
			getchar();
			break;
		case 5:
			if (L)
				printf("\t\t单链表的长度为：%d\t\n", ListLength(L));
			else
				printf("\t\t单链表不存在！\n");
			getchar();
			printf("\n\t\t(按任意键继续..........)\n");
			getchar();
			break;
		case 6:
			if (L)
			{
				printf("\t\t请输入想要获得的数据元素的位序：");
				scanf("%d", &i);
				if (GetElem(L, i, e) == ERROR)
					printf("\n\t\t该数据元素不在范围内！\n");
				else
					printf("\n\t\t单链表L中第%d个数据元素的值是：%d\n", i, e);
			}
			else
			{
				printf("\t\t单链表不存在！\n");
			}
			getchar();
			printf("\n\t\t(按任意键继续..........)\n");
			getchar();
			break;
		case 7:
			if (L)
			{
				printf("\t\t请输入需要查找的数据元素：");
				scanf("%d", &e);
				if (LocateElem(L, e) == ERROR)
					printf("\n\t\t数据元素的值查找失败！该元素不存在！\n");
				else
					printf("\n\t\t该数据元素值在单链表中为第%d个元素！\n", LocateElem(L, e));
			}
			else
			{
				printf("\t\t单链表不存在！\n");
			}
			getchar();
			printf("\n\t\t(按任意键继续..........)\n");
			getchar();
			break;
		case 8:
			if (L)
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
				printf("\t\t单链表不存在！\n");
			}
			getchar();
			printf("\n\t\t(按任意键继续..........)\n");
			getchar();
			break;
		case 9:
			if (L)
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
				printf("\t\t单链表不存在！\n");
			}
			getchar();
			printf("\n\t\t(按任意键继续..........)\n");
			getchar();
			break;
		case 10:
			if (L)
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
					printf("\n\t\t新数据元素插入成功！单链表的长度为：%d\n", ListLength(L));
			}
			else
			{
				printf("\t\t单链表不存在！\n");
			}
			getchar();
			printf("\n\t\t(按任意键继续..........)\n");
			getchar();
			break;
		case 11:
			if (L)
			{
				printf("\t\t请输入要删除的数据元素在单链表中的位序：");
				scanf("%d", &i);
				if (ListDelete(L, i, e) == ERROR)
					printf("\n\t\t数据元素删除失败！\n");
				else
					printf("\n\t\t数据元素删除成功！被删除的数据元素为%d,表的长度是%d !\n", e, ListLength(L));
			}
			else
			{
				printf("\t\t单链表不存在！\n");
			}
			getchar();
			printf("\n\t\t(按任意键继续..........)\n");
			getchar();
			break;
		case 12:
			//printf("\n----ListTrabverse功能待实现！\n");
			if (L)
			{
				if (ListTraverse(L))
				{
					if (ListLength(L) == 0)
						printf("\t\t单链表是空表！\n");
				}
				else
				{
					printf("\t\t单链表不存在！\n");
				}
			}
			else
			{
				printf("\t\t单链表不存在！\n");
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