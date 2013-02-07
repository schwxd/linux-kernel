#include <stdio.h>
#include <stdlib.h>

#define FALSE -1
#define TRUE  0
#define Status int
#define EQ(x, y) (x == y)
#define LT(x, y) (x < y)
#define BT(x, y) (x > y)

Status SearchBST(BiTree T, KeyType key, BiTree f, BiTree p)
{
	if (!T) {
		p = f;
		return FALSE;
	}

	if (EQ(key, T->data.key)) {
		f = T;
		return TRUE;
	} else if (LT(key, T->data.key))
		return SearchBST(T->lchild, key, T, p);
	else
		return SearchBST(T->rchild, key, T, p);
}

Status InsertBST(BiTree T, ElemType e)
{
	BiTree p, tmp;
	if (SearchBST(T, e.key, NULL, p) == FALSE) {
		tmp = (BiTree)malloc(sizeof(struct BiTNode));
		tmp->data = e;
		tmp->lchild = tmp->rchild = NULL;
		if (p == NULL)
			T = tmp;
		else if (LT(e.key, T->data.key)) {
			p->lchild = tmp;
		} else {
			p->rchild = tmp;
		}
		return TRUE;
	}
	return FALSE;
}
