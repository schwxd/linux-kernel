/* 二叉树的二叉线索存储表示 */
enum PointerTag {
	Link = 0,
	Thread,
};

struct BiThrNode {
	TElemType data;
	struct BiThrNode *lchild, *rchild;
	enum PointerTag ltag, rtag;
};

typedef struct BiThrNode *BiThrTree;


/* 二叉树的二叉线索存储的基本操作 */

/* 按先序输入二叉线索树中结点的值,构造二叉线索树T */
/* 0(整型)/空格(字符型)表示空结点 */
Status CreateBiThrTree(BiThrTree *T)
{
	TElemType e;

#if CHAR
	scanf("%c", &e);
#endif
/* #if INT */
/* 	scanf("%d", &e); */
/* #endif */

	if (e == Nil) {
		*T = NULL;
	} else {
		*T = (BiThrTree)malloc(sizeof(struct BiThrNode));
		if (!*T)
			return ERROR;
		(*T)->data = e;
		CreateBiThrTree(&(*T)->lchild);
		if ((*T)->lchild)
			(*T)->ltag = Link;
		CreateBiThrTree(&(*T)->rchild);
		if ((*T)->rchild)
			(*T)->rtag = Link;
	}

	return OK;
}

/* 中序遍历进行中序线索化。算法6.7 */
BiThrTree pre;    /* 全局变量,始终指向刚刚访问过的结点 */
void InThreading(BiThrTree p)
{
	if (p) {
		/* 递归左子树线索化 */
		InThreading(p->lchild);
		if (!p->lchild) {		/* 没有左孩子 */
			p->ltag = Thread;	/* 前驱线索 */
			p->lchild = pre;	/* 左孩子指针指向前驱 */
		}
		if (!pre->rchild) {		/* 前驱没有右孩子 */
			pre->rtag = Thread;	/* 后继线索 */
			pre->rchild = p;	/* 前驱右孩子指针指向后继(当前结点p) */
		}
		pre = p;			/* 保持pre指向p的前驱 */
		InThreading(p->rchild);		/* 递归右子树线索化 */
	}
}

/* 中序遍历二叉树T,并将其中序线索化,Thrt指向头结点。算法6.6 */
Status InOrderThreading(BiThrTree *Thrt,BiThrTree T)
{
	*Thrt = (BiThrTree)malloc(sizeof(struct BiThrNode));
	if (!*Thrt)
		return ERROR;
	(*Thrt)->ltag = Link;
	(*Thrt)->rtag = Thread;
	(*Thrt)->rchild = *Thrt;
	if (!T) {
		/* 若二叉树空，则左指针回指 */
		(*Thrt)->lchild = (*Thrt);
	} else {
		(*Thrt)->lchild = T;
		pre = *Thrt;
		/* 中序遍历进行中序线索化 */
		InThreading(T);

		/* 最后一个结点线索化 */
		pre->rchild = (*Thrt);
		pre->rtag = Thread;

		(*Thrt)->rchild = pre;
	}

	return OK;
}

/* 中序遍历二叉线索树T(头结点)的非递归算法。算法6.5 */
Status InOrderTraverse_Thr(BiThrTree T,Status(*Visit)(TElemType))
{
	BiThrTree p;
	p = T->lchild;				/* p指向根结点 */
	while (p != T) {			/* 空树或遍历结束时,p==T */
//		while (p->lchild == Link)
		while (p->ltag == Link)
			p = p->lchild;
		if (!Visit(p->data))		/* 访问其左子树为空的结点 */
			return ERROR;
		printf("ltag:%d lchild:%c, rtag:%d rchild:%c\n", p->ltag, p->lchild->data, p->rtag, p->rchild->data);

		while (p->rtag == Thread && p->rchild != T) {
			p = p->rchild;
			Visit(p->data);		/* 访问后继结点 */
			printf("ltag:%d lchild:%c, rtag:%d rchild:%c\n", p->ltag, p->lchild->data, p->rtag, p->rchild->data);
		}
		p = p->rchild;
	}
	return OK;
}
