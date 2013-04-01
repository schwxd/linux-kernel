#include "../include/common.h"
//单链表
//循环链表
//静态链表
//双向链表

typedef int ElemType;

//线性表的动态分配顺序存储结构
#define LIST_INIT_SIZE 20
#define LISTINCREMENT  2
typedef struct {
	ElemType *elem; //存储空间基址
	int length;
	int listsize;
}Sqlist;

//线性表的单链表存储结构
#if 0
typedef struct LNode {
	ElemType data;
	struct LNode *next;
} *LinkList;
#endif

//线性表的静态单链表存储结构
#define MAXSIZE 100
typedef struct SLNode {
	ElemType data;
	int cur;
} component, SLinkList[MAXSIZE];

//线性表的双向链表存储结构
typedef struct DuLNode {
	ElemType data;
	struct DulNode *next;
	struct DulNode *prior;
} DulNode, *DuLinkList;

//带头结点的线性链表类型
typedef struct LNode {
	ElemType data;
	struct LNode *next;
} LNode, *Link, *Position;

typedef struct LinkList {
	Link head, tail;
	int length;
} LinkList;

/*************************************************************************************/
//具有实用意义的线性链表(struct LinkList)的24个基本操作
Status MakeNode(Link *p, ElemType e);
void FreeNode(Link *p);
Status InitList(LinkList *L);
Status ClearList(LinkList *L);
Status DestroyList(LinkList *L);
Status InsFirst(LinkList *L, Link h, Link s);
Status DelFirst(LinkList *L, Link h, Link *q);
Status Append(LinkList *L, Link s);
Status Remove(LinkList *L, Link *q);
Status InsBefore(LinkList *L, Link *p, Link s);
Status InsAfter(LinkList *L, Link *p, Link s);
Status SetCurElem(Link p, ElemType e);
ElemType GetCurElem(Link p);
Status ListEmpty(LinkList L);
int ListLength(LinkList L);
Position GetHead(LinkList L);
Position GetLast(LinkList L);
Position PriorPos(LinkList L, Link p);
//Position NextPos(LinkList L, Link p);
Position NextPos(Link p);
Status LocatePos(LinkList L, int i, Link *p);
Position LocateElem(LinkList L, ElemType e, Status (* compare)(ElemType, ElemType));
Status ListTraverse(LinkList L, void (* visit)(ElemType));

Status MakeNode(Link *p, ElemType e)
{
	*p = (Link)malloc(sizeof(LNode));
	if (!*p)
		return ERROR;
	(*p)->data = e;
	return OK;
}	

void FreeNode(Link *p)
{
	free(*p);
	*p = NULL;
}

Status InitList(LinkList *L)
{
	Link p;
	p = (Link)malloc(sizeof(struct LNode));
	if (p) {
		p->next = NULL;
		L->head = L->tail = p;
		L->length = 0;
		return OK;
	}
	return ERROR;
}

Status ClearList(LinkList *L)
{
	Link *p;

	if (L->head != L->tail) {
		*p =  L->head->next;
		while (*p != L->tail) {
			L->head->next = NULL;
			free(L->head);
			L->head = *p;
			*p = (*p)->next;
		}
		L->head->next = NULL;
		free(L->head);
	}
	L->head = L->tail;
	L->length = 0;
	return OK;
}

/* 销毁线性链表L，L不再存在 */
Status DestroyList(LinkList *L)
{
	Link p;
	if (L->head != L->tail) {
		p = L->head->next;
		while (p != L->tail) {
			L->head->next = NULL;
			free(L->head);
			L->head = p;
			p = p->next;
		}
		L->head->next = NULL;
		free(L->head);
		free(L->tail);
	}
	L = NULL;
	return OK;
}

/* h指向L的一个结点，把h当做头结点，将s所指结点插入在第一个结点之前 */
Status InsFirst(LinkList *L, Link h, Link s)
{
	s->next = h->next;
	h->next = s;
	L->length++;
	if (L->tail == h)
		L->tail = s;
	return OK;
}

/* h指向L的一个结点，把h当做头结点，删除链表中的第一个结点并以q返回。 */
/* 若链表为空(h指向尾结点)，q=NULL，返回FALSE */
Status DelFirst(LinkList *L, Link h, Link *q)
{
	if (h == L->tail) {
		*q = NULL;
		return FALSE;
	}

	*q = h->next;
	if (h->next == L->tail) {
		L->tail = h;
	}
	h->next = h->next->next;
	L->length--;
	return OK;
}

/* 将指针s(s->data为第一个数据元素)所指(彼此以指针相链,以NULL结尾)的 */
/* 一串结点链接在线性链表L的最后一个结点之后,并改变链表L的尾指针指向新 */
/* 的尾结点 */
Status Append(LinkList *L, Link s)
{
	while (s != NULL) {
		L->tail->next = s;
		L->tail = s;
		L->length++;
		s = s->next;
	}
	return OK;
}

/* 删除线性链表L中的尾结点并以q返回，改变链表L的尾指针指向新的尾结点 */
Status Remove(LinkList *L, Link *q)
{
	Link p;
	p = L->head;

	if (L->length == 0) {
		*q = NULL;
		q = NULL;
		return FALSE;
	}

	*q = L->tail;
	while (p->next != L->tail)
		p = p->next;
	L->tail = p;
	L->tail->next = NULL;
	L->length--;
	return OK;
}

/* 已知p指向线性链表L中的一个结点，将s所指结点插入在p所指结点之前， */
/* 并修改指针p指向新插入的结点 */
Status InsBefore(LinkList *L, Link *p, Link s)
{
	Link q;
	q = PriorPos(*L, *p);
	if (!q)
		q = L->head; 
	s->next = *p;
	q->next = s;
	*p = s;
	L->length++;
	return OK;
}

/* 已知p指向线性链表L中的一个结点，将s所指结点插入在p所指结点之后， */
/* 并修改指针p指向新插入的结点 */
Status InsAfter(LinkList *L, Link *p, Link s)
{
	s->next = (*p)->next;
	(*p)->next = s;
	if (*p == L->tail) {
		L->tail = s;
	}
	*p = s;
	L->length++;
	return OK;
}

/* 已知p指向线性链表中的一个结点，用e更新p所指结点中数据元素的值 */
Status SetCurElem(Link p, ElemType e)
{
	p->data = e;
	return OK;
}

/* 已知p指向线性链表中的一个结点，返回p所指结点中数据元素的值 */
ElemType GetCurElem(Link p)
{
	return p->data;
}

/* 若线性链表L为空表，则返回TRUE，否则返回FALSE */
Status ListEmpty(LinkList L)
{
	return L.length == 0 ? TRUE : FALSE;
}

/* 返回线性链表L中元素个数 */
int ListLength(LinkList L)
{
	return L.length;
}

/* 返回线性链表L中头结点的位置 */
Position GetHead(LinkList L)
{
	return L.head;
}

/* 返回线性链表L中最后一个结点的位置 */
Position GetLast(LinkList L)
{
	return L.tail;
}

/* 已知p指向线性链表L中的一个结点，返回p所指结点的直接前驱的位置 */
/* 若无前驱，则返回NULL */
Position PriorPos(LinkList L, Link p)
{
	Link q;
	q = L.head->next;

	if (q == p)
		return FALSE;

	while (q->next != p)
		q = q->next;
	
	return q;
}

/* 已知p指向线性链表L中的一个结点，返回p所指结点的直接后继的位置 */
/* 若无后继，则返回NULL */
Position NextPos(Link p)
{
	return p->next;
}

/* 返回p指示线性链表L中第i个结点的位置，并返回OK，i值不合法时返回ERROR */
/* i=0为头结点 */
Status LocatePos(LinkList L, int i, Link *p)
{
	if (i<0 || i>L.length)
		return ERROR;

	*p = L.head;
	while (i-- >= 0)
		*p = (*p)->next;

	return OK;
}

/* 返回线性链表L中第1个与e满足函数compare()判定关系的元素的位置， */
/* 若不存在这样的元素，则返回NULL */
Position LocateElem(LinkList L, ElemType e, Status (* compare)(ElemType, ElemType))
{
	Link q;
	q = L.head->next;

	while (q != NULL && compare(q->data, e) != TRUE)
		q = q->next;
	return q;
}

/* 依次对L的每个数据元素调用函数visit()。一旦visit()失败，则操作失败 */
Status ListTraverse(LinkList L, void (* visit)(ElemType))
{
	Link p;
	p = L.head->next;

	while (p) {
		visit(p->data);
		p = p->next;
	}

	if (p == NULL)
		return OK;
	else
		return ERROR;
}
/* 已知L为有序线性链表，将元素e按非降序插入在L中。（用于一元多项式） */
Status OrderInsert(LinkList *L,ElemType e,int (*comp)(ElemType,ElemType))
{
	Link p, q, node;
	q = L->head;
	p = L->head->next;

	while (p && comp(p->data, e) < 0) {
		q = p;
		p = p->next;
	}

	if (MakeNode(&node, e) != OK) {
		printf("make node error\n");
	}
	node->next = p;
	q->next = node;
	L->length++;
	if (!p)
		L->tail = node;
	return OK;
}

/* 若升序链表L中存在与e满足判定函数compare()取值为0的元素，则q指示L中 */
/* 第一个值为e的结点的位置，并返回TRUE；否则q指示第一个与e满足判定函数 */
/* compare()取值>0的元素的前驱的位置。并返回FALSE。（用于一元多项式） */
Status LocateElemP(LinkList L,ElemType e,Position *q,int(*compare)(ElemType,ElemType))
{
	Link p, pp;
	p = L.head;

	do {
		pp = p;
		p = p->next;
		
	} while (p && compare(p->data, e));

	if (!p || compare(p->data, e) > 0) {
		*q = pp;
		return FALSE;
	} else {
		*q = p;
		return TRUE;
	}
}

/*************************************************************************************/
/* main2-6.c 检验bo2-6.cpp的主程序 */
Status compare(ElemType c1,ElemType c2) /* c1等于c2 */
{
  if(c1==c2)
    return TRUE;
  else
    return FALSE;
}

int cmp(ElemType a,ElemType b)
{ /* 根据a<、=或>b,分别返回-1、0或1 */
  if(a==b)
    return 0;
  else
    return (a-b)/abs(a-b);
}

void visit(ElemType c)
{
  printf("%d ",c);
}

//void main()
int main(int argc, char **argv)
{
	Link p,h;
	LinkList L;
	Status i;
	int j,k;
	i=InitList(&L);
	if(!i) /* 初始化空的线性表L不成功 */
		exit(FALSE); /* 退出程序运行 */
	for(j=1;j<=2;j++)
	{
		MakeNode(&p,j); /* 生成由p指向、值为j的结点 */
		InsFirst(&L,L.tail,p); /* 插在表尾 */
	}
	printf("输出链表：");
	ListTraverse(L,visit); /* 输出L */

	OrderInsert(&L,0,cmp); /* 按升序插在有序表头 */
	for(j=0;j<=3;j++)
	{
		i=LocateElemP(L,j,&p,cmp);
		if(i)
			printf("链表中有值为%d的元素。\n",p->data);
		else
			printf("链表中没有值为%d的元素。\n",j);
	}
	printf("输出链表：");
	ListTraverse(L,visit); /* 输出L */
	for(j=1;j<=4;j++)
	{
		printf("删除表头结点：");
		DelFirst(&L,L.head,&p); /* 删除L的首结点，并以p返回 */
		if(p)
			printf("%d\n",GetCurElem(p));
		else
			printf("表空，无法删除 p=%u\n",p);
	}
	printf("L中结点个数=%d L是否空 %d(1:空 0:否)\n",ListLength(L),ListEmpty(L));
	MakeNode(&p,10);
	p->next=NULL; /* 尾结点 */
	for(j=4;j>=1;j--)
	{
		MakeNode(&h,j*2);
		h->next=p;
		p=h;
	} /* h指向一串5个结点，其值依次是2 4 6 8 10 */
	Append(&L,h); /* 把结点h链接在线性链表L的最后一个结点之后 */
	OrderInsert(&L,12,cmp); /* 按升序插在有序表尾头 */
	OrderInsert(&L,7,cmp); /* 按升序插在有序表中间 */
	printf("输出链表：");
	ListTraverse(L,visit); /* 输出L */
	for(j=1;j<=2;j++)
	{
		p=LocateElem(L,j*5,compare);
		if(p)
			printf("L中存在值为%d的结点。\n",j*5);
		else
			printf("L中不存在值为%d的结点。\n",j*5);
	}
	for(j=1;j<=2;j++)
	{
		LocatePos(L,j,&p); /* p指向L的第j个结点 */
		h=PriorPos(L,p); /* h指向p的前驱 */
		if(h)
			printf("%d的前驱是%d。\n",p->data,h->data);
		else
			printf("%d没前驱。\n",p->data);
	}
	k=ListLength(L);
	for(j=k-1;j<=k;j++)
	{
		LocatePos(L,j,&p); /* p指向L的第j个结点 */
		h=NextPos(p); /* h指向p的后继 */
		if(h)
			printf("%d的后继是%d。\n",p->data,h->data);
		else
			printf("%d没后继。\n",p->data);
	}
	printf("L中结点个数=%d L是否空 %d(1:空 0:否)\n",ListLength(L),ListEmpty(L));
	p=GetLast(L); /* p指向最后一个结点 */
	SetCurElem(p,15); /* 将最后一个结点的值变为15 */
	printf("第1个元素为%d 最后1个元素为%d\n",GetCurElem(GetHead(L)->next),GetCurElem(p));
	MakeNode(&h,10);
	InsBefore(&L,&p,h); /* 将10插到尾结点之前，p指向新结点 */
	p=p->next; /* p恢复为尾结点 */
	MakeNode(&h,20);
	InsAfter(&L,&p,h); /* 将20插到尾结点之后 */
	k=ListLength(L);
	printf("依次删除表尾结点并输出其值：");
	for(j=0;j<=k;j++)
	{
		i=Remove(&L,&p);
		if(!i) /* 删除不成功 */
			printf("删除不成功 p=%u\n",p);
		else
			printf("%d ",p->data);
	}
	MakeNode(&p,29); /* 重建具有1个结点(29)的链表 */
	InsFirst(&L,L.head,p);
	DestroyList(&L); /* 销毁线性链表L */
	printf("销毁线性链表L之后: L.head=%u L.tail=%u L.len=%d\n",L.head,L.tail,L.length);
}
