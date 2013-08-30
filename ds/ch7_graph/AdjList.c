/* 图的邻接表存储表示 */
#define MAX_VERTEX_NUM 20			
typedef enum {DG, DN, AG, AN} GraphKind;	/* {有向图,有向网,无向图,无向网} */
typedef struct ArcNode {
	int adjvex;			/* 该弧所指向的顶点的位置 */
	struct ArcNode *nextarc;	/* 指向下一条弧的指针 */
	InfoType *info;			/* 网的权值指针 */
} ArcNode;				/* 表结点 */

typedef struct VNode {
	VertexType data;		/* 顶点信息 */
	ArcNode *firstarc;		/* 第一个表结点的地址,指向第一条依附该顶点的弧的指针 */
} VNode, AdjList[MAX_VERTEX_NUM];	/* 头结点 */

typedef struct {
	AdjList vertices;
	int vexnum, arcnum;		/* 图的当前顶点数和弧数 */
	GraphKind kind;			/* 图的种类标志 */
} ALGraph;



/* 图的邻接表存储的基本操作(15个) */

/* 初始条件: 图G存在,u和G中顶点有相同特征 */
/* 操作结果: 若G中存在顶点u,则返回该顶点在图中位置;否则返回-1 */
int LocateVex(ALGraph G, VertexType u)
{
	int i;
	for (i = 0; i < G.vexnum; ++i) {
		if (strcmp(G.vertices[i].data, u) == 0)
			return i;
	}
	return -1;
}

/* 采用邻接表存储结构,构造没有相关信息的图G(用一个函数构造4种图) */
Status CreateGraph(ALGraph *G)
{
	int i, k1, k2;
	VertexType va, vb;

	printf("input graph kind: ");
	scanf("%d", &(*G).kind);
	if ((*G).kind < 0)
		exit(ERROR);

	printf("input vertex num, arcnum:");
	scanf("%d,%d", &(*G).vexnum, &(*G).arcnum);

	printf("input all the vertex\n");
	for (i = 0; i < G->vexnum; ++i) {
		scanf("%s", &(*G).vertices[i].data);
	}

	printf("input all the arcs\n");
	for (i = 0; i < (*G).arcnum; ++i) {
		printf("input the No. %d arc:", i);
		scanf("%s %s", va, vb);
		k1 = LocateVex(*G, va);
		k2 = LocateVex(*G, vb);
		ArcNode *arc = (ArcNode *)malloc(sizeof(struct ArcNode));
		arc->adjvex = k2;
		arc->info = NULL;
		if ((*G).vertices[k1].firstarc == NULL)
			(*G).vertices[k1].firstarc = arc;
		else {
			arc->nextarc = (*G).vertices[k1].firstarc->nextarc;
			(*G).vertices[k1].firstarc->nextarc = arc;
		}
		if ((*G).kind == AG || (*G).kind == AN) {
			ArcNode *arc2 = (ArcNode *)malloc(sizeof(struct ArcNode));
			arc2->adjvex = k1;
			arc2->info = NULL;
			if ((*G).vertices[k2].firstarc == NULL)
				(*G).vertices[k2].firstarc = arc2;
			else {
				arc2->nextarc = (*G).vertices[k2].firstarc->nextarc;
				(*G).vertices[k2].firstarc->nextarc = arc2;
			}
		}
	}
}

/* 初始条件: 图G存在。操作结果: 销毁图G */
void DestroyGraph(ALGraph *G)
{
	int i;
	ArcNode *arc1, *arc2;

	for (i = 0; i < (*G).vexnum; ++i) {
		if ((*G).vertices[i].firstarc == NULL)
			continue;
		else {
			for (arc1 = (*G).vertices[i].firstarc->nextarc; arc1 != NULL;) {
				arc2 = arc1->nextarc;
				if (arc1->info)
					free(arc1->info);
				free(arc1);
				arc1 = arc2;
			}
		}
	}
}

/* 初始条件: 图G存在,v是G中某个顶点的序号。操作结果: 返回v的值 */
VertexType* GetVex(ALGraph G, int u)
{
	if (u < 0 || u >= G.vexnum)
		return ERROR;
	return &(G.vertices[u].data);
}

/* 初始条件: 图G存在,v是G中某个顶点 */
/* 操作结果: 对v赋新值value */
Status PutVex(ALGraph *G, VertexType v, VertexType value)
{
	int i;
	i = LocateVex(*G, v);
	if (i < 0)
		return ERROR;

	strcpy((*G).vertices[i].data, value);
//	(*G).vertices[i].data = value;
	return OK;
}

/* 初始条件: 图G存在,v是G中某个顶点 */
/* 操作结果: 返回v的第一个邻接顶点的序号。若顶点在G中没有邻接顶点,则返回-1 */
int FirstAdjVex(ALGraph G, VertexType v)
{
	int i;
	VertexType adj;
	i = LocateVex(G, v);
	if (i < 0)
		return ERROR;
	if (G.vertices[i].firstarc != NULL)
		return G.vertices[i].firstarc->adjvex;
}

/* 初始条件: 图G存在,v是G中某个顶点,w是v的邻接顶点 */
/* 操作结果: 返回v的(相对于w的)下一个邻接顶点的序号。 */
/*           若w是v的最后一个邻接点,则返回-1 */
int NextAdjVex(ALGraph G, VertexType v, VertexType w)
{
	int i, j;
	VertexType t;
	ArcNode *p;

	i = LocateVex(G, v);
	j = LocateVex(G, w);
	if (i < 0 || j < 0)
		return ERROR;

	p = G.vertices[i].firstarc;
	while (p && p->adjvex != j)
		p = p->nextarc;

	if (p && p->nextarc) {
		/* t = p->nextarc->nextarc; */
		/* return LocateVex(G, t); */
		return p->nextarc->adjvex;
	}
	return -1;
}

/* 初始条件: 图G存在,v和图中顶点有相同特征 */
/* 操作结果: 在图G中增添新顶点v(不增添与顶点相关的弧,留待InsertArc()去做) */
void InsertVex(ALGraph *G, VertexType v)
{
	strcpy((*G).vertices[(*G).vexnum].data, v);
//	(*G).vertices[(*G).vexnum].data = v;
	(*G).vertices[(*G).vexnum].firstarc = NULL;
	(*G).vexnum++;
}

/* 初始条件: 图G存在,v是G中某个顶点 */
/* 操作结果: 删除G中顶点v及其相关的弧 */
Status DeleteVex(ALGraph *G, VertexType v)
{

}

/* 初始条件: 图G存在,v和w是G中两个顶点 */
/* 操作结果: 在G中增添弧<v,w>,若G是无向的,则还增添对称弧<w,v> */
Status InsertArc(ALGraph *G, VertexType v, VertexType w)
{
	int i, j;
	i = LocateVex(*G, v);
	j = LocateVex(*G, w);
	if (i < 0 || j < 0)
		return ERROR;

	ArcNode *arc  = (ArcNode *)malloc(sizeof(ArcNode));
	arc->adjvex = j;
	arc->info = NULL;

	if ((*G).vertices[i].firstarc == NULL)
		(*G).vertices[i].firstarc = arc;
	else {
		arc->nextarc = (*G).vertices[i].firstarc;
		(*G).vertices[i].firstarc = arc;
	}

	if ((*G).kind == AG || (*G).kind == AN) {
		ArcNode *arc2 = (ArcNode *)malloc(sizeof(ArcNode));
		arc2->adjvex = i;
		arc2->info = NULL;
		if ((*G).vertices[j].firstarc == NULL)
			(*G).vertices[j].firstarc = arc2;
		else {
			arc2->nextarc = (*G).vertices[j].firstarc;
			(*G).vertices[j].firstarc = arc2;
		}
	}
	return OK;
}

/* 初始条件: 图G存在,v和w是G中两个顶点 */
/* 操作结果: 在G中删除弧<v,w>,若G是无向的,则还删除对称弧<w,v> */
Status DeleteArc(ALGraph *G, VertexType v, VertexType w)
{
	int i, j;
	ArcNode *p, *q;
	i = LocateVex(*G, v);
	j = LocateVex(*G, w);
	if (i < 0 || j < 0)
		return ERROR;

	p = (*G).vertices[i].firstarc;
	while (p && p->adjvex != j) {
		q = p;
		p = p->nextarc;
	}
	if (!p)
		return ERROR;
	else if (p == (*G).vertices[i].firstarc) {
		(*G).vertices[i].firstarc = p->nextarc;
		if (p->info)
			free(p->info);
		free(p);
	} else {
		q->nextarc = p->nextarc;
		if (p->info)
			free(p->info);
		free(p);
	}

	if ((*G).kind == AG || (*G).kind == AN) {
		p = (*G).vertices[j].firstarc;
		while (p && p->adjvex != i) {
			q = p;
			p = p->nextarc;
		}
		if (!p)
			return ERROR;
		else if (p == (*G).vertices[j].firstarc) {
			(*G).vertices[j].firstarc = p->nextarc;
			if (p->info)
				free(p->info);
			free(p);
		} else {
			q->nextarc = p->nextarc;
			if (p->info)
				free(p->info);
			free(p);
		}
	}
}

int visited[MAX_VERTEX_NUM];
Status (*VisitFunc)(VertexType);	/* 函数变量 */
/* 从第v个顶点出发递归地深度优先遍历图G。算法7.5 */
void DFS(ALGraph G, int v)
{
	ArcNode *p;
	if (visited[v] == TRUE)
		return;
	else
		visited[v] = TRUE;
	VisitFunc(G.vertices[v].data);

	p = G.vertices[v].firstarc;
	while (p) {
		DFS(G, p->adjvex);
		p = p->nextarc;
	}
}

/* 对图G作深度优先遍历。算法7.4 */
void DFSTraverse(ALGraph G, void(*Visit)(char*))
{
	int i;
	VisitFunc = Visit;
	for (i = 0; i < G.vexnum; i++)
		visited[i] = FALSE;

	for (i = 0; i < G.vexnum; i++)
		DFS(G, i);
}

typedef VNode QElemType;
#include "../ch3/linkqueue.h"
#include "../ch3/linkqueue.c"
LinkQueue Q;
int visited[MAX_VERTEX_NUM];
/*按广度优先非递归遍历图G。使用辅助队列Q和访问标志数组visited。算法7.6 */
void BFSTraverse(ALGraph G, void(*Visit)(char*))
{
	int i;
	VNode *node;
	ArcNode *p;

	for (i = 0; i < G.vexnum; i++)
		visited[i] = FALSE;
	InitQueue(&Q);

	Visit(G.vertices[0].data);
	visited[0] = TRUE;
	EnQueue(&Q, G.vertices[0]);
	while (!QueueEmpty(Q)) {
		DeQueue(&Q, node);
		p = node->firstarc;
		while (p) {
			Visit(G.vertices[p->adjvex].data);
			visited[p->adjvex] = TRUE;
			EnQueue(&Q, G.vertices[p->adjvex]);
			p = p->nextarc;
		}
	}
}

/* 输出图的邻接矩阵G */
void Display(ALGraph G)
{
}
