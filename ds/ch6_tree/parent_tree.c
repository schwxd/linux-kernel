//typedef char TElemType;
//#include "../include/common.h"
/* 树的双亲表存储表示 */
#define MAX_TREE_SIZE 100
typedef struct {
	TElemType data;
	int parent;	/* 双亲位置域 */
} PTNode;

typedef struct {
	PTNode node[MAX_TREE_SIZE];
	int n;		/* 结点数 */
} PTree;

/* 树的双亲表存储的基本操作(14个) */
/* 操作结果: 构造空树T */
Status InitTree(PTree *T)
{
	(*T).n = 0;
	return OK;
}

/* 由于PTree是定长类型,无法销毁 */
void DestroyTree()
{ 
}

/* 操作结果: 构造树T */
/* typedef struct { */
/* 	PTNode node; */
/* 	int num; */
/* } QElemType; */

typedef struct {
	TElemType data;
	int parent;
} QElemType;

#include "../ch3/linkqueue.h"
#include "../ch3/linkqueue.c"

Status CreateTree(PTree *T)
{
	int i, l, num;
	TElemType e;
	char c[MAX_TREE_SIZE];
	LinkQueue q;
	QElemType qe;

	InitQueue(&q);
	printf("input the root node: ");
	scanf("%c%*c", &e);
	if (e == Nil) {
		T->n = 0;
	} else {
		qe.data = e;
		qe.parent = -1;
		EnQueue(&q, qe);
		while (!QueueEmpty(q)) {
			DeQueue(&q, &qe);
			num = T->n++;
			T->node[num].data = qe.data;
			T->node[num].parent = qe.parent;
			
			printf("input the children of node %c ", qe.data);
			gets(c);
			l = strlen(c);
			for (i=0;i<l;i++) {
				qe.data = c[i];
				qe.parent = num;
				EnQueue(&q, qe);
			}
		}
	}
	return OK;
}

/* 初始条件: 树T存在。操作结果: 若T为空树,则返回TRUE,否则返回FALSE */
Status TreeEmpty(PTree T)
{ 
	if (T.n == 0)
		return TRUE;
	else
		return FALSE;
}

/* 初始条件: 树T存在。操作结果: 返回T的深度 */
int TreeDepth(PTree T)
{
	int i, parent = -2, num, depth = 1;

	if (T.n == 0)
		return 0;
	if (T.n == 1)
		return 1;

	for (i=0;i<T.n;i++) {
		if (T.node[i].parent > parent) {
			parent = T.node[i].parent;
			num = i;
		}
	}
	if (parent == -1)
		return 1;
	else {
		while (T.node[num].parent != -1) {
			num = T.node[num].parent;
			depth++;
		}
	}
	return depth;
}

/* 初始条件: 树T存在。操作结果: 返回T的根 */
TElemType Root(PTree T)
{
	int i;
	for (i=0;i<T.n;i++) {
		if (T.node[i].parent == -1)
			return T.node[i].data;
	}
	return Nil;
}

/* 初始条件: 树T存在,i是树T中结点的序号。操作结果: 返回第i个结点的值 */
TElemType Value(PTree T,int i)
{
	return T.node[i].data;
}

/* 初始条件: 树T存在,cur_e是树T中结点的值。操作结果: 改cur_e为value */
Status Assign(PTree *T,TElemType cur_e,TElemType value)
{
	int i;
	for (i=0;i<T->n;i++) {
		if (T->node[i].data == cur_e) {
			T->node[i].data = value;
			return OK;
		}
	}
	return ERROR;
}

/* 初始条件: 树T存在,cur_e是T中某个结点 */
/* 操作结果: 若cur_e是T的非根结点,则返回它的双亲,否则函数值为＂空＂ */
TElemType Parent(PTree T,TElemType cur_e)
{ 
	int i;
	for (i=0;i<T.n;i++) {
		if (T.node[i].data == cur_e) {
			i = T.node[i].parent;
			return T.node[i].data;
		}
	}
	return Nil;
}

/* 初始条件: 树T存在,cur_e是T中某个结点 */
/* 操作结果: 若cur_e是T的非叶子结点,则返回它的最左孩子,否则返回＂空＂ */
TElemType LeftChild(PTree T,TElemType cur_e)
{
	int i, num;
	for (i=0;i<T.n;i++) {
		if (T.node[i].data == cur_e) {
			num = i;
		}
	}
	for (i=0;i<T.n;i++) {
		if (T.node[i].parent == num)
			return T.node[i].data;
	}
	return Nil;
   
}

/* 初始条件: 树T存在,cur_e是T中某个结点 */
/* 操作结果: 若cur_e有右(下一个)兄弟,则返回它的右兄弟,否则返回＂空＂ */
TElemType RightSibling(PTree T,TElemType cur_e)
{
	int i, parent, num;
	for (i=0;i<T.n;i++) {
		if (T.node[i].data == cur_e) {
			parent = T.node[i].parent;
			num = i;
		}
	}
	for (i=0;i<T.n;i++) {
		if (T.node[i].parent == parent && i > num)
			return T.node[i].data;
	}
	return Nil;
}

/* 输出树T。加 */
Status Print(PTree T)
{
	return OK;
}

/* 初始条件: 树T存在,p是T中某个结点,1≤i≤p所指结点的度+1,非空树c与T不相交 */
/* 操作结果: 插入c为T中p结点的第i棵子树 */
Status InsertChild(PTree *T,TElemType p,int i,PTree c)
{
	return OK;
}

/* 初始条件: 树T存在,p是T中某个结点,1≤i≤p所指结点的度 */
/* 操作结果: 删除T中结点p的第i棵子树 */
void DeleteChild(PTree *T,TElemType p,int i)
{ 
}

/* 初始条件:二叉树T存在,Visit是对结点操作的应用函数 */
/* 操作结果:层序遍历树T,对每个结点调用函数Visit一次且仅一次 */
void TraverseTree(PTree T,void(*Visit)(TElemType))
{
	int i;
	for (i = 0; i < T.n; i++) {
		Visit(T.node[i].data);
	}
}
