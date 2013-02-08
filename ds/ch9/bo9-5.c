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

Status DeleteBST(BiTree T, KeyType key)
{
	BiTree *p, l, r, tmp;

	if (T == NULL)
		return FALSE;

	if (SearchBST(T, key, NULL, *p) == TRUE) {
		if (!(*p)->lchild && !(*p)->rchild) {
			free(*p);
			//p的双亲节点怎么确定？
			//不需要知道双亲节点，直接覆盖p的值就可以。
		} else if ((*p)->lchild && !(*p)->rchild) {
			(*p) = (*p)->lchild;
		} else if (!(*p)->lchild && (*p)->rchild) {
			(*p) = (*p)->rchild;
		} else {
			r = (*p)->rchild;
			tmp = l = (*p)->lchild;
			while (tmp->rchild) {
				tmp = tmp->rchild;
			}
			tmp->rchild = r;
			(*p)->lchild = l;
		}
		return TRUE;
	}
	return FALSE;
}
