/*二叉树的二叉链表存储表示*/
typedef struct BiTNode {
	TElemType data;
	struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

Status InitBiTree(BiTree *T);
Status DestroyBiTree(BiTree *T);
void CreateBiTree(BiTree *T);
Status BiTreeEmpty(BiTree T);
int BiTreeDepth(BiTree T);
TElemType Root(BiTree T);
TElemType Value(BiTree p);
void Assign(BiTree p, TElemType value);
TElemType Parent(BiTree T, TElemType e);
BiTree Point(BiTree T,TElemType s);
TElemType LeftChild(BiTree T, TElemType e);
TElemType RightChild(BiTree T, TElemType e);
TElemType LeftSibling(BiTree T, TElemType e);
TElemType RightSibling(BiTree T, TElemType e);
Status InsertChild(BiTree p,int LR,BiTree c);
Status DeleteChild(BiTree p, int LR);
void PreOrderTraverse(BiTree T, Status(* Visit)(TElemType));
void InOrderTraverse(BiTree T, Status(* Visit)(TElemType));
void PostOrderTraverse(BiTree T, Status(* Visit)(TElemType));
void LevelOrderTraverse(BiTree T, Status(* Visit)(TElemType));




