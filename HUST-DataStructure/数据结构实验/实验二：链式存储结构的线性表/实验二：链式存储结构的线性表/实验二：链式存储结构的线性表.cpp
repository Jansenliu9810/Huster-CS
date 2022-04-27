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
typedef int ElemType; //����Ԫ�����Ͷ���

/*-------page 22 on textbook -------*/
#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
typedef struct LNode{  //��������ʽ�ṹ���Ķ���
	ElemType data;
	struct LNode* next;
}LNode,*LinkList;
/*-----page 19 on textbook ---------*/
status InitList(LinkList& L)
// ������L�����ڣ�����һ���յĵ���������OK�����򷵻�INFEASTABLE��
{
	// �������ﲹ����룬��ɱ�������
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
// ���������L���ڣ����ٵ�����L���ͷ�����Ԫ�صĿռ䣬����OK�����򷵻�INFEASTABLE��
{
	// �������ﲹ����룬��ɱ�������
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
// ���������L���ڣ�ɾ��������L�е�����Ԫ�أ�����OK�����򷵻�INFEASTABLE��
{
	// �������ﲹ����룬��ɱ�������
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
// ���������L���ڣ��жϵ�����L�Ƿ�Ϊ�գ��վͷ���TRUE�����򷵻�FALSE�����������L�����ڣ�����INFEASTABLE��
{
	// �������ﲹ����룬��ɱ�������
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
// ���������L���ڣ����ص�����L�ĳ��ȣ����򷵻�INFEASTABLE��
{
	// �������ﲹ����룬��ɱ�������
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
// ���������L���ڣ���ȡ������L�ĵ�i��Ԫ�أ�������e�У�����OK�����i���Ϸ�������ERROR�����������L�����ڣ�����INFEASTABLE��
{
	// �������ﲹ����룬��ɱ�������
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
// ���������L���ڣ�����Ԫ��e�ڵ�����L�е�λ����ţ����e�����ڣ�����ERROR����������L������ʱ������INFEASTABLE��
{
	// �������ﲹ����룬��ɱ�������
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
// ���������L���ڣ���ȡ������L��Ԫ��e��ǰ����������pre�У�����OK�����û��ǰ��������ERROR�����������L�����ڣ�����INFEASTABLE��
{
	// �������ﲹ����룬��ɱ�������
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
// ���������L���ڣ���ȡ������LԪ��e�ĺ�̣�������next�У�����OK�����û�к�̣�����ERROR�����������L�����ڣ�����INFEASTABLE��
{
	// �������ﲹ����룬��ɱ�������
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
// ���������L���ڣ���Ԫ��e���뵽������L�ĵ�i��Ԫ��֮ǰ������OK��������λ�ò���ȷʱ������ERROR�����������L�����ڣ�����INFEASTABLE��
{
	// �������ﲹ����룬��ɱ�������
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
// ���������L���ڣ�ɾ��������L�ĵ�i��Ԫ�أ���������e�У�����OK����ɾ��λ�ò���ȷʱ������ERROR�����������L�����ڣ�����INFEASTABLE��
{
	// �������ﲹ����룬��ɱ�������
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
// ���������L���ڣ�������ʾ�������е�Ԫ�أ�ÿ��Ԫ�ؼ��һ�񣬷���OK�����������L�����ڣ�����INFEASTABLE��
{
	// �������ﲹ����룬��ɱ�������
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
	printf("���������һ����������в���1-99��������0���˳�����\n");
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
		printf("      ��ѡ����Ĳ���[0~12]:");
		scanf("%d", &op);
		switch (op) {
		case 1:
			//printf("\n----IntiList���ܴ�ʵ�֣�\n");
			if (!L)
			{
				if (InitList(L) == OK)
					printf("\t\t���������ɹ���\n");
				else
				{
					printf("\t\t��������ʧ�ܣ�\n");
				}
			}
			else
			{
				printf("\t\t�������Ѵ��ڣ������ԭ�е�������ٽ��г�ʼ�������������\n");
			}
			getchar();
			printf("\n\t\t(�����������..........)\n");
			getchar();
			break;
		case 2:
			if (L)
			{
				if (DestroyList(L) == OK)
					printf("\t\t������ɾ���ɹ���\n");
				else
					printf("\t\t������ɾ��ʧ�ܣ�\n");
			}
			else
			{
				printf("\t\t���������ڣ�\n");
			}
			getchar();
			printf("\n\t\t(�����������..........)\n");
			getchar();
			break;
		case 3:
			if (L)
			{
				if (ClearList(L) == OK)
					printf("\t\t��������ճɹ���\n");
				else
					printf("\t\t���������ʧ�ܣ�\n");
			}
			else
			{
				printf("\t\t���������ڣ�\n");
			}
			getchar();
			printf("\n\t\t(�����������..........)\n");
			getchar();
			break;
		case 4:
			if (L)
			{
				if (ListEmpty(L) == TRUE)
					printf("\t\t�˵�����Ϊ�գ�\n");
				else
					printf("\t\t�˵�����Ϊ�գ�\n");
			}
			else
			{
				printf("\t\t���������ڣ�\n");
			}
			getchar();
			printf("\n\t\t(�����������..........)\n");
			getchar();
			break;
		case 5:
			if (L)
				printf("\t\t������ĳ���Ϊ��%d\t\n", ListLength(L));
			else
				printf("\t\t���������ڣ�\n");
			getchar();
			printf("\n\t\t(�����������..........)\n");
			getchar();
			break;
		case 6:
			if (L)
			{
				printf("\t\t��������Ҫ��õ�����Ԫ�ص�λ��");
				scanf("%d", &i);
				if (GetElem(L, i, e) == ERROR)
					printf("\n\t\t������Ԫ�ز��ڷ�Χ�ڣ�\n");
				else
					printf("\n\t\t������L�е�%d������Ԫ�ص�ֵ�ǣ�%d\n", i, e);
			}
			else
			{
				printf("\t\t���������ڣ�\n");
			}
			getchar();
			printf("\n\t\t(�����������..........)\n");
			getchar();
			break;
		case 7:
			if (L)
			{
				printf("\t\t��������Ҫ���ҵ�����Ԫ�أ�");
				scanf("%d", &e);
				if (LocateElem(L, e) == ERROR)
					printf("\n\t\t����Ԫ�ص�ֵ����ʧ�ܣ���Ԫ�ز����ڣ�\n");
				else
					printf("\n\t\t������Ԫ��ֵ�ڵ�������Ϊ��%d��Ԫ�أ�\n", LocateElem(L, e));
			}
			else
			{
				printf("\t\t���������ڣ�\n");
			}
			getchar();
			printf("\n\t\t(�����������..........)\n");
			getchar();
			break;
		case 8:
			if (L)
			{
				printf("\t\t������һ������Ԫ�����ڲ��Ҵ�ǰ�����Ԫ�أ�");
				scanf("%d", &current_e);
				if (PriorElem(L, current_e, pre_e) == ERROR)
					printf("\n\t\t������Ԫ��û��ǰ�����Ԫ�أ�\n");
				else
					printf("\n\t\t������Ԫ��%d��ǰ�����Ԫ��Ϊ��%d\n", current_e, pre_e);
			}
			else
			{
				printf("\t\t���������ڣ�\n");
			}
			getchar();
			printf("\n\t\t(�����������..........)\n");
			getchar();
			break;
		case 9:
			if (L)
			{
				printf("\t\t������һ������Ԫ�����ڲ��Ҵ˺�̽��Ԫ�أ�");
				scanf("%d", &current_e);
				if (NextElem(L, current_e, next_e) == ERROR)
					printf("\n\t\t������Ԫ��û�к�̽��Ԫ�أ�\n");
				else
					printf("\n\t\t������Ԫ��%d�ĺ�̽��Ԫ��Ϊ��%d\n", current_e, next_e);
			}
			else
			{
				printf("\t\t���������ڣ�\n");
			}
			getchar();
			printf("\n\t\t(�����������..........)\n");
			getchar();
			break;
		case 10:
			if (L)
			{
				printf("\t\t�������������Ԫ�ص�λ�ã�");
				scanf("%d", &i);
				getchar();
				printf("\t\t�������������Ԫ�ص�ֵ��");
				scanf("%d", &e);
				getchar();
				if (ListInsert(L, i, e) == ERROR)
					printf("\n\t\t������Ԫ�ز���ʧ�ܣ�\n");
				else
					printf("\n\t\t������Ԫ�ز���ɹ���������ĳ���Ϊ��%d\n", ListLength(L));
			}
			else
			{
				printf("\t\t���������ڣ�\n");
			}
			getchar();
			printf("\n\t\t(�����������..........)\n");
			getchar();
			break;
		case 11:
			if (L)
			{
				printf("\t\t������Ҫɾ��������Ԫ���ڵ������е�λ��");
				scanf("%d", &i);
				if (ListDelete(L, i, e) == ERROR)
					printf("\n\t\t����Ԫ��ɾ��ʧ�ܣ�\n");
				else
					printf("\n\t\t����Ԫ��ɾ���ɹ�����ɾ��������Ԫ��Ϊ%d,��ĳ�����%d !\n", e, ListLength(L));
			}
			else
			{
				printf("\t\t���������ڣ�\n");
			}
			getchar();
			printf("\n\t\t(�����������..........)\n");
			getchar();
			break;
		case 12:
			//printf("\n----ListTrabverse���ܴ�ʵ�֣�\n");
			if (L)
			{
				if (ListTraverse(L))
				{
					if (ListLength(L) == 0)
						printf("\t\t�������ǿձ�\n");
				}
				else
				{
					printf("\t\t���������ڣ�\n");
				}
			}
			else
			{
				printf("\t\t���������ڣ�\n");
			}
			getchar();
			printf("\n\t\t(�����������..........)\n");
			getchar();
			break;
		case 0:
			break;
		}//end of switch
	}//end of while
	printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
}//end of main()
/*--------page 23 on textbook --------------------*/