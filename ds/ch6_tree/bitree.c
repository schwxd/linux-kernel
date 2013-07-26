#define TElemType char

#include "../include/common.h"
#include "bitree.h"

/* 操作结果: 构造空二叉树T */
Status InitBiTree(BiTree *T)
{
	*T = NULL;
	return OK;
}

/* 初始条件: 二叉树T存在。操作结果: 销毁二叉树T */
Status DestroyBiTree(BiTree *T)
{
	if (*T) {
		if ((*T)->lchild)
			DestroyBiTree(&(*T)->lchild);
		if ((*T)->rchild)
			DestroyBiTree(&(*T)->rchild);
		free(*T);
		*T = NULL;
	}
}

/* 算法6.4:按先序次序输入二叉树中结点的值（可为字符型或整型，在主程中 */
/* 定义），构造二叉链表表示的二叉树T。变量Nil表示空（子）树。有改动 */
#define CHAR
void CreateBiTree(BiTree *T)
{
	TElemType ch;
#ifdef CHAR
	scanf("%c", &ch);
#endif
#ifdef INT
	scanf("%d", &ch);
#endif

	if (ch == Nil)
		*T = NULL;
	else {
		*T = (struct BiTNode *)malloc(sizeof(struct BiTNode));
		if (*T == NULL)
			exit(OVERFLOW);
		(*T)->data = ch;
		CreateBiTree(&(*T)->lchild);
		CreateBiTree(&(*T)->rchild);
	}
}


/* 初始条件: 二叉树T存在 */
/* 操作结果: 若T为空二叉树,则返回TRUE,否则FALSE */
Status BiTreeEmpty(BiTree T)
{
	if (T == NULL)
		return TRUE;
	else
		return FALSE;
}

//Status ClearBiTree(BiTree T);
#define ClearBiTree DestroyBiTree

/* 初始条件: 二叉树T存在。操作结果: 返回T的深度 */
int BiTreeDepth(BiTree T)
{
	int i, j;
	i = j = 0;

	if (T == NULL)
		return 0;

	if (T->lchild)
		i = BiTreeDepth(T->lchild);

	if (T->rchild)
		j = BiTreeDepth(T->rchild);

	return i>j?i+1:j+1;
}

/* 初始条件: 二叉树T存在。操作结果: 返回T的根 */
TElemType Root(BiTree T)
{
	if (T)
		return T->data;
}

/* 初始条件: 二叉树T存在，p指向T中某个结点 */
/* 操作结果: 返回p所指结点的值 */
TElemType Value(BiTree p)
{
	if (p)
		return p->data;
}

/* 给p所指结点赋值为value */
void Assign(BiTree p, TElemType value)
{
	if (p)
		p->data = value;
}

/* 初始条件: 二叉树T存在,e是T中某个结点 */
/* 操作结果: 若e是T的非根结点,则返回它的双亲,否则返回＂空＂ */
TElemType Parent2(BiTree T, TElemType e)
{
	TElemType *p;

	p = NULL;
	if (T->data == e)
		return NULL;

	if (T->lchild) {
		if (T->lchild->data == e)
			return T->data;
		*p = Parent(T->lchild, e);
		if (*p)
			return *p;
	}

	if (T->rchild) {
		if (T->rchild->data == e)
			return T->data;
		*p = Parent(T->rchild, e);
		if (*p)
			return *p;
	}

	return *p;
}

typedef BiTree QElemType;
#include "../ch3/linkqueue.h"
#include "../ch3/linkqueue.c"
TElemType Parent(BiTree T, TElemType e)
{
	LinkQueue q;
	QElemType a;

	if (T) {
		InitQueue(&q);
		EnQueue(&q, T);
		while (!QueueEmpty(q)) {
			DeQueue(&q, &a);
//			if (a->lchild->data == e || a->rchild->data == e) {
			if ((a->lchild && a->lchild->data == e) || (a->rchild && a->rchild->data == e)) {
				return a->data;
			}
			if (a->lchild)
				EnQueue(&q, T->lchild);
			if (a->rchild)
				EnQueue(&q, T->rchild);
		}
	}
	return NULL;
}

/* 返回二叉树T中指向元素值为s的结点的指针。另加 */
BiTree Point(BiTree T,TElemType s)
{
	LinkQueue q;
	QElemType e;

	if (T) {
		InitQueue(&q);
		EnQueue(&q, T);
		while (!QueueEmpty(q)) {
			DeQueue(&q, &e);
			if (e->data == s)
				return e;

			if (e->lchild)
				EnQueue(&q, e->lchild);
			if (e->rchild)
				EnQueue(&q, e->rchild);
		}
	}
}


/* 初始条件: 二叉树T存在,e是T中某个结点 */
/* 操作结果: 返回e的左孩子。若e无左孩子,则返回＂空＂ */
TElemType LeftChild(BiTree T, TElemType e)
{
	BiTree p;
	if (T) {
		p = Point(T, e);
		if (p && p->lchild)
			return p->lchild->data;
	}
	return NULL;
}

/* 初始条件: 二叉树T存在,e是T中某个结点 */
/* 操作结果: 返回e的右孩子。若e无右孩子,则返回＂空＂ */
TElemType RightChild(BiTree T, TElemType e)
{
	BiTree p;
	if (T) {
		p = Point(T, e);
		if (p && p->rchild)
			return p->rchild->data;
	}

	return NULL;
}

/* 初始条件: 二叉树T存在,e是T中某个结点 */
/* 操作结果: 返回e的左兄弟。若e是T的左孩子或无左兄弟,则返回＂空＂ */
TElemType LeftSibling(BiTree T, TElemType e)
{
	TElemType p;
	BiTree a;
	if (T) {
		p = Parent(T, e);
		a = Point(T, p);
		if (a->rchild && a->rchild->data == e && a->lchild)
			return a->lchild->data;
	}
	return NULL;
}

/* 初始条件: 二叉树T存在,e是T中某个结点 */
/* 操作结果: 返回e的右兄弟。若e是T的右孩子或无右兄弟,则返回＂空＂ */
TElemType RightSibling(BiTree T, TElemType e)
{
	TElemType p;
	BiTree a;
	if (T) {
		p = Parent(T, e);
		a = Point(T, p);
		if (a->lchild && a->lchild->data == e && a->rchild)
			return a->rchild->data;
	}
	return NULL;
}

/* 初始条件: 二叉树T存在,p指向T中某个结点,LR为0或1,非空二叉树c与T */
/*           不相交且右子树为空 */
/* 操作结果: 根据LR为0或1,插入c为T中p所指结点的左或右子树。p所指结点的 */
/*           原有左或右子树则成为c的右子树 */
//Status InsertChild(BiTree T, BiTNode *p, int lr, BiTree c)
Status InsertChild(BiTree p,int LR,BiTree c) /* 形参T无用 */
{
	BiTree t;
	if (p) {
		if (LR == 0) {
			c->rchild = p->lchild;
			p->lchild = c;
		}
		else if (LR == 1) {
			c->rchild = p->rchild;
			p->rchild = c;
		}
		return OK;
	}
	return ERROR;
}

/* 初始条件: 二叉树T存在,p指向T中某个结点,LR为0或1 */
/* 操作结果: 根据LR为0或1,删除T中p所指结点的左或右子树 */
Status DeleteChild(BiTree p, int LR) /* 形参T无用 */
{
	if (p) {
		if (LR == 0) {
			DestroyBiTree(&p->lchild);
			p->lchild = NULL;// not necessary
		} else {
			DestroyBiTree(&p->rchild);
			p->rchild = NULL;
		}
		return OK;
	}
	return ERROR;
}

/* 初始条件: 二叉树T存在,Visit是对结点操作的应用函数。算法6.1，有改动 */
/* 操作结果: 先序递归遍历T,对每个结点调用函数Visit一次且仅一次 */
void PreOrderTraverse(BiTree T, Status(* Visit)(TElemType))
{
	if (T) {
		Visit(T->data);
		if (T->lchild != NULL)
			PreOrderTraverse(T->lchild, Visit);
		if (T->rchild != NULL)
			PreOrderTraverse(T->rchild, Visit);
	}
}

/* 初始条件: 二叉树T存在,Visit是对结点操作的应用函数 */
/* 操作结果: 中序递归遍历T,对每个结点调用函数Visit一次且仅一次 */
void InOrderTraverse(BiTree T, Status(* Visit)(TElemType))
{
	if (T) {
		if (T->lchild)
			InOrderTraverse(T->lchild, Visit);
		Visit(T->data);
		if (T->rchild)
			InOrderTraverse(T->rchild, Visit);
	}
}

typedef BiTree SElemType;
#include "../ch3/sqstack.h"
#include "../ch3/sqstack.c"
/* 采用二叉链表存储结构，Visit是对数据元素操作的应用函数。算法6.3 */
/* 中序遍历二叉树T的非递归算法(利用栈)，对每个数据元素调用函数Visit */
Status InOrderTraverse1(BiTree T, Status(*Visit)(TElemType))
{
	SqStack S;
	InitStack(&S);
	while (T || !StackEmpty(S)) {
		if (T) {
			/* 根指针进栈,遍历左子树 */
			Push(&S, T);
			T = T->lchild;
		} else {
			/* 根指针退栈,访问根结点,遍历右子树 */
			Pop(&S, &T);
			if (!Visit(T->data))
				return ERROR;
			T = T->rchild;
		}
	}
	return OK;	
}

/* 我自己写的，可以运行，代码不如示例的简洁 */
void InOrderTraverse1_xdwang(BiTree T,Status(*Visit)(TElemType))
{
	SqStack sq;
	SElemType s;

	if (T) {
		InitStack(&sq);
		Push(&sq, T);
		while (GetTop(sq, &s) && s->lchild) {
			Push(&sq, s->lchild);
		}

		while (GetTop(sq, &s)) {
			Pop(&sq, &s);
			Visit(s->data);
			if (s->rchild) {
				Push(&sq, s->rchild);
				while (GetTop(sq, &s) && s->lchild) {
					Push(&sq, s->lchild);
				}
			}
		}
	}
}


/* 采用二叉链表存储结构，Visit是对数据元素操作的应用函数。算法6.2 */
/* 中序遍历二叉树T的非递归算法(利用栈)，对每个数据元素调用函数Visit */
Status InOrderTraverse2(BiTree T,Status(*Visit)(TElemType))
{
	SqStack S;
	BiTree p;
	InitStack(&S);
	Push(&S, T);

	while (!StackEmpty(S)) {
		while (GetTop(S, &p) && p)
			Push(&S, p->lchild);
		Pop(&S, &p);
		if (!StackEmpty(S)) {
			Pop(&S, &p);
			if (!Visit(p->data))
				return ERROR;
			Push(&S, p->rchild);
		}
	}
	return OK;
}


/* 初始条件: 二叉树T存在,Visit是对结点操作的应用函数 */
/* 操作结果: 后序递归遍历T,对每个结点调用函数Visit一次且仅一次 */
void PostOrderTraverse(BiTree T, Status(* Visit)(TElemType))
{
	if (T) {
		if (T->lchild)
			PostOrderTraverse(T->lchild, Visit);
		if (T->rchild)
			PostOrderTraverse(T->rchild, Visit);
		Visit(T->data);
	}	
}

/* 初始条件：二叉树T存在,Visit是对结点操作的应用函数 */
/* 操作结果：层序递归遍历T(利用队列),对每个结点调用函数Visit一次且仅一次 */
void LevelOrderTraverse(BiTree T, Status(* Visit)(TElemType))
{
	LinkQueue q;
	QElemType e;
	if (T) {
		InitQueue(&q);
		EnQueue(&q, T);
		while (!QueueEmpty(q)) {
			DeQueue(&q, &e);
			Visit(e->data);
			if (e->lchild)
				EnQueue(&q, e->lchild);
			if (e->rchild)
				EnQueue(&q, e->rchild);
		}
	}
}
