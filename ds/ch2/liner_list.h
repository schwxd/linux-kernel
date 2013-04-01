#ifndef LINER_LIST_H
#define LINER_LIST_H
//单链表
//循环链表
//静态链表
//双向链表
#include "../include/common.h"

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
#endif
