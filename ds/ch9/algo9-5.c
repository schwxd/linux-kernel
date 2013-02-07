typedef int KeyType;

typedef struct {
	KeyType key;
	int others;
} ElemType;

typedef ElemType TElemType;

typedef struct BiTNode {
	TElemType data;
	struct BiTNode *lchild, *rchild;
} *BiTree;

#include "bo9-5.c"

int main(int argc, char *argv[])
{
	return 0;
}


