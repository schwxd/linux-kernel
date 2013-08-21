/* typedef int VRType; */
/* typedef char* VertexType; */
/* typedef char InfoType; */
/* #define MAX_INFO 10 */
/* #define INFINITE -1 */
/* #include "../include/common.h" */

/* 图的数组(邻接矩阵)存储表示 */
#define MAX_VERTEX_NUM 20 			/* 最大顶点个数 */
typedef enum {DG, DN, AG, AN} GraphKind;	/* {有向图,有向网,无向图,无向网} */
typedef struct {
	VRType adj;				/* 顶点关系类型。对无权图，用1(是)或0(否)表示相邻否； */
						/* 对带权图，则为权值类型 */
	InfoType *info;				/* 该弧相关信息的指针(可无) */
} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef struct {
	VertexType vexs[MAX_VERTEX_NUM];	/* 顶点向量 */
	AdjMatrix arcs;				/* 邻接矩阵 */
	int vexnum, arcnum;			/* 图的当前顶点数和弧数 */
	GraphKind kind;				/* 图的种类标志 */
} MGraph;


/* 图的数组(邻接矩阵)存储的基本操作(20个) */

/* 初始条件:图G存在,u和G中顶点有相同特征 */
/* 操作结果:若G中存在顶点u,则返回该顶点在图中位置;否则返回-1 */
int LocateVex(MGraph G,VertexType u)
{ 
	int i;
	for (i=0;i<G.vexnum;i++)
		if (strcmp(G.vexs[i], u) == 0)
			return i;
	return -1;
}

/* 采用数组(邻接矩阵)表示法,由文件构造没有相关信息的无向图G */
Status CreateFAG(MGraph *G)
{
/*	
	int i, j, k, vexnum, arcnum;
	VertexType tail, head;
	fscanf("%d", &vexnum);	
	fscanf("%d", &arcnum);
	for(i=0;i<vexnum;i++) {
		fscanf("%d", &(*G).vexs[i]);
	}
	for(i=0;i<arcnum;i++) {
		fscanf("%c%c",&tail, &head); 
		j = LocateVex(*G, tail);
		k = LocateVex(*G, head);
		(*G).arcs[i][j].adj = (*G).arcs[j][i].adj = 1;
	}
	return OK;
*/

	int i, j, k;
	char filename[20];
	FILE *graphfile;
	VertexType va, vb;

	printf("input the graph data file (f7-1.dat)");
	scanf("%s", filename);
	if (strlen(filename) == 0) {
		printf("filename is null, use default\n");
		strcpy(filename, "f7-1.dat");	
	}
	printf("filename:%s\n", filename);
	graphfile = fopen(filename, "r");
	if (!graphfile) {
		printf("cannot open file %s\n", filename);
		return ERROR;
	}
	fscanf(graphfile, "%d%d", &(*G).vexnum, &(*G).arcnum);
	for (i=0;i<(*G).vexnum;i++) {
		fscanf(graphfile, "%s", (*G).vexs[i]);
	}
	for (i=0;i<(*G).vexnum;i++)
		for (j=0;j<(*G).vexnum;j++) {
			(*G).arcs[i][j].adj = 0;
			(*G).arcs[i][j].info = NULL;
		}

	for (k=0;k<(*G).arcnum;k++) {
///		fscanf(graphfile, "%c %c", va, vb);
		fscanf(graphfile, "%s %s", va, vb);
		i = LocateVex(*G, va);
		j = LocateVex(*G, vb);	
		(*G).arcs[i][j].adj = (*G).arcs[j][i].adj = 1;
	}
	fclose(graphfile);
	(*G).kind = AG;
	return OK;
}

/* 采用数组(邻接矩阵)表示法,构造有向图G */
Status CreateDG(MGraph *G)
{
	int i, j, k, len;
	int hasinfo;
	char info[MAX_INFO];
	VertexType va, vb;
	printf("请输入有向图G的顶点数,弧数,弧是否含其它信息(是:1,否:0): ");
	scanf("%d, %d, %d", &(*G).vexnum, &(*G).arcnum, &hasinfo);
	printf("input all the %d vertex:\n", (*G).vexnum);
	for (i=0;i<(*G).vexnum;i++) {
		scanf("%s", (*G).vexs[i]);
	}

	for (i=0;i<(*G).vexnum;i++)
		for(j=0;j<(*G).vexnum;j++) {
			(*G).arcs[i][j].adj = 0;
			(*G).arcs[i][j].info = NULL;
		}
	printf("input all the %d arcs:\n", (*G).arcnum);
	for(k=0;k<(*G).arcnum;k++){
		scanf("%s %s",va, vb );
		i = LocateVex(*G, va);
		j = LocateVex(*G, vb);
		(*G).arcs[i][j].adj = 1;
		if (hasinfo) {
			printf("input arc info:");
			scanf("%s", info);
			len = strlen(info);
			if (len) {
				(*G).arcs[i][j].info = (char*)malloc((len+1) * sizeof(char));
				strncpy((*G).arcs[i][j].info, info, len);
			}
		} else
			(*G).arcs[i][j].info = NULL;
	}
	(*G).kind = DG;
	return OK;

}

/* 采用数组(邻接矩阵)表示法,构造有向网G */
Status CreateDN(MGraph *G)
{ 
	int i, j, k, len, weight;
	int hasinfo;
	char info[MAX_INFO];
	VertexType va, vb;
	printf("请输入有向网G的顶点数,弧数,弧是否含其它信息(是:1,否:0): ");
	scanf("%d, %d, %d", &(*G).vexnum, &(*G).arcnum, &hasinfo);
	printf("input all the %d vertex:\n", (*G).vexnum);
	for (i=0;i<(*G).vexnum;i++) {
		scanf("%s", (*G).vexs[i]);
	}

	for (i=0;i<(*G).vexnum;i++)
		for(j=0;j<(*G).vexnum;j++) {
			(*G).arcs[i][j].adj = INFINITE;
			(*G).arcs[i][j].info = NULL;
		}
	printf("input all the %d arcs:\n", (*G).arcnum);
	for(k=0;k<(*G).arcnum;k++){
		scanf("%s %s, %d",va, vb, &weight );
		i = LocateVex(*G, va);
		j = LocateVex(*G, vb);
		(*G).arcs[i][j].adj = weight;
		if (hasinfo) {
			printf("input arc info:");
			scanf("%s", info);
			len = strlen(info);
			if (len) {
				(*G).arcs[i][j].info = (char*)malloc((len+1) * sizeof(char));
				strncpy((*G).arcs[i][j].info, info, len);
			}
		} else
			(*G).arcs[i][j].info = NULL;
	}
	(*G).kind = DN;
	return OK;
}

/* 采用数组(邻接矩阵)表示法,构造无向图G */
Status CreateAG(MGraph *G)
{ 
	int i, j, k, len;
	int hasinfo;
	char info[MAX_INFO];
	VertexType va, vb;
	printf("请输入无向图G的顶点数,弧数,弧是否含其它信息(是:1,否:0): ");
	scanf("%d, %d, %d", &(*G).vexnum, &(*G).arcnum, &hasinfo);
	printf("input all the %d vertex:\n", (*G).vexnum);
	for (i=0;i<(*G).vexnum;i++) {
		scanf("%s", (*G).vexs[i]);
	}

	for (i=0;i<(*G).vexnum;i++)
		for(j=0;j<(*G).vexnum;j++) {
			(*G).arcs[i][j].adj = 0;
			(*G).arcs[i][j].info = NULL;
		}
	printf("input all the %d arcs:\n", (*G).arcnum);
	for(k=0;k<(*G).arcnum;k++){
		scanf("%s %s",va, vb );
		i = LocateVex(*G, va);
		j = LocateVex(*G, vb);
		(*G).arcs[i][j].adj = (*G).arcs[j][i].adj = 1;
		if (hasinfo) {
			printf("input arc info:");
			scanf("%s", info);
			len = strlen(info);
			if (len) {
				(*G).arcs[i][j].info = (*G).arcs[j][i].info = (char*)malloc((len+1) * sizeof(char));
				strncpy((*G).arcs[i][j].info, info, len);
			}
		} else
			(*G).arcs[i][j].info = NULL;
	}
	(*G).kind = AG;
	return OK;


}

/* 采用数组(邻接矩阵)表示法,构造无向网G。算法7.2 */
Status CreateAN(MGraph *G)
{ 
	int i, j, k, len, weight;
	int hasinfo;
	char info[MAX_INFO];
	VertexType va, vb;
	printf("请输入无向网G的顶点数,弧数,弧是否含其它信息(是:1,否:0): ");
	scanf("%d, %d, %d", &(*G).vexnum, &(*G).arcnum, &hasinfo);
	printf("input all the %d vertex:\n", (*G).vexnum);
	for (i=0;i<(*G).vexnum;i++) {
		scanf("%s", (*G).vexs[i]);
	}

	for (i=0;i<(*G).vexnum;i++)
		for(j=0;j<(*G).vexnum;j++) {
			(*G).arcs[i][j].adj = INFINITE;
			(*G).arcs[i][j].info = NULL;
		}
	printf("input all the %d arcs:\n", (*G).arcnum);
	for(k=0;k<(*G).arcnum;k++){
		scanf("%s %s, %d",va, vb, &weight );
		i = LocateVex(*G, va);
		j = LocateVex(*G, vb);
		(*G).arcs[i][j].adj = (*G).arcs[j][i].adj = weight;
		if (hasinfo) {
			printf("input arc info:");
			scanf("%s", info);
			len = strlen(info);
			if (len) {
				(*G).arcs[i][j].info = (*G).arcs[j][i].info = (char*)malloc((len+1) * sizeof(char));
				strncpy((*G).arcs[i][j].info, info, len);
			}
		} else
			(*G).arcs[i][j].info = NULL;
	}
	(*G).kind = AN;
	return OK;
}

/* 采用数组(邻接矩阵)表示法,构造图G。算法7.1 */
Status CreateGraph(MGraph *G)
{ 
	switch ((*G).kind)
	{
		case DG:
			return CreateDG(G);
		case DN:
			return CreateDN(G);
		case AG:
			return CreateAG(G);
		case AN:
			return CreateAN(G);
	}
	return ERROR;
}

/* 初始条件: 图G存在。操作结果: 销毁图G */
void DestroyGraph(MGraph *G)
{ 
	int i, j;
	for (i = 0; i < (*G).arcnum; ++i) {
		for (j = 0; j < (*G).arcnum; ++j) {
			if ((*G).arcs[i][j].info) {
				free((*G).arcs[i][j].info);
			}
			
		}
		
	}
	free(*(*G).arcs);
	free(G);	
}

/* 初始条件: 图G存在，v是G中某个顶点的序号。操作结果: 返回v的值 */
VertexType* GetVex(MGraph G,int v)
{
	if (v < 0 || v >= G.vexnum)
		return NULL;
	return &G.vexs[v];
}

/* 初始条件: 图G存在，v是G中某个顶点。操作结果: 对v赋新值value */
Status PutVex(MGraph *G,VertexType v,VertexType value)
{
	int i;
	i = LocateVex(*G, v);
	if (i != -1) {
		strncpy((*G).vexs[i], value, MAX_NAME);
//		(*G).vexs[i] = value;
		return OK;
	} else
		return ERROR;
}

/* 初始条件: 图G存在,v是G中某个顶点 */
/* 操作结果: 返回v的第一个邻接顶点的序号。若顶点在G中没有邻接顶点,则返回-1 */
int FirstAdjVex(MGraph G,VertexType v)
{
	int i, j;
	i = LocateVex(G, v);
	if (i != -1) {
		for (j = 0; j < G.vexnum; ++j) {
			if (G.arcs[i][j].adj == 1)
				return j;
		}
	}
	return -1;
}

/* 初始条件: 图G存在,v是G中某个顶点,w是v的邻接顶点 */
/* 操作结果: 返回v的(相对于w的)下一个邻接顶点的序号, */
/*	       若w是v的最后一个邻接顶点,则返回-1 */
int NextAdjVex(MGraph G,VertexType v,VertexType w)
{
	int i, j, k;
	i = LocateVex(G, v);
	j = LocateVex(G, w);
	if (i != -1 && j != -1) {
		for (k = j+1; k < G.vexnum; k++) {
			if (G.arcs[i][k].adj == 1)
				return k;
		}
	}
	return -1;
}

/* 初始条件: 图G存在,v和图G中顶点有相同特征 */
/* 操作结果: 在图G中增添新顶点v(不增添与顶点相关的弧,留待InsertArc()去做) */
void InsertVex(MGraph *G,VertexType v)
{
	int pos;
	pos = (*G).vexnum + 1;
	if (pos > MAX_VERTEX_NUM)
		exit(OVERFLOW);
//	(*G).vexs[pos] = v;
	strncpy((*G).vexs[pos], v, MAX_NAME);
	(*G).vexnum++;
}

/* 初始条件: 图G存在,v是G中某个顶点。操作结果: 删除G中顶点v及其相关的弧 */
Status DeleteVex(MGraph *G,VertexType v)
{
	int i, j;
	i = LocateVex(*G, v);
	if (i == -1)
		return ERROR;
	for (j = 0; j < (*G).vexnum; ++j) {
		//row
		if ((*G).arcs[i][j].adj == 1)
			(*G).arcs[i][j].adj = 0;
		if ((*G).arcs[i][j].info != NULL) {
			free((*G).arcs[i][j].info);
			(*G).arcs[i][j].info = NULL;
		}

		//colume
		if ((*G).arcs[j][i].adj == 1)
			(*G).arcs[j][i].adj = 0;
		if ((*G).arcs[j][i].info != NULL) {
			free((*G).arcs[j][i].info);
			(*G).arcs[j][i].info = NULL;
		}
	}
	return OK;
}

/* 初始条件: 图G存在,v和w是G中两个顶点 */
/* 操作结果: 在G中增添弧<v,w>,若G是无向的,则还增添对称弧<w,v> */
Status InsertArc(MGraph *G,VertexType v,VertexType w)
{
	int i, j;
	i = j = -1;
	i = LocateVex(*G, v);
	j = LocateVex(*G, w);
	if (i == -1 || j == -1)
		return ERROR;

	(*G).arcs[i][j].adj = 1;
	(*G).arcnum++;
	if ((*G).kind == AG) {
		(*G).arcs[j][i].adj = 1;
		(*G).arcnum++;
	}
	return OK;
}

/* 初始条件: 图G存在,v和w是G中两个顶点 */
/* 操作结果: 在G中删除弧<v,w>,若G是无向的,则还删除对称弧<w,v> */
Status DeleteArc(MGraph *G,VertexType v,VertexType w)
{
	int i, j;
	i = j = -1;
	i = LocateVex(*G, v);
	j = LocateVex(*G, w);
	if (i == -1 || j == -1)
		return ERROR;

	(*G).arcs[i][j].adj = 0;
	(*G).arcnum--;
	if ((*G).kind == AG) {
		(*G).arcs[j][i].adj = 0;
		(*G).arcnum--;
	}
	return OK;
}

Boolean visited[MAX_VERTEX_NUM];	/* 访问标志数组(全局量) */
Status (*VisitFunc)(VertexType);	/* 函数变量 */
/* 从第v个顶点出发递归地深度优先遍历图G。算法7.5 */
void DFS(MGraph G,int v)
{
	int w;
	visited[v] = TRUE;
	VisitFunc(G.vexs[v]);
	for (w = FirstAdjVex(G, G.vexs[v]); w >= 0; w = NextAdjVex(G, G.vexs[v], G.vexs[w])) {
		if (visited[w] == FALSE)
			DFS(G, w);
	}
}

/* 初始条件: 图G存在,Visit是顶点的应用函数。算法7.4 */
/* 操作结果: 从第1个顶点起,深度优先遍历图G,并对每个顶点调用函数Visit */
/*           一次且仅一次。一旦Visit()失败,则操作失败 */
void DFSTraverse(MGraph G,Status(*Visit)(VertexType))
{
	int i;
	for (i = 0; i < MAX_VERTEX_NUM; i++)
		visited[i] = FALSE;

	for (i = 0; i < MAX_VERTEX_NUM; ++i) {
		DFS(G, i);
	}
}

#define QElemType int
#include "../ch3/linkqueue.h"
#include "../ch3/linkqueue.c"
/* 初始条件: 图G存在,Visit是顶点的应用函数。算法7.6 */
/* 操作结果: 从第1个顶点起,按广度优先非递归遍历图G,并对每个顶点调用函数 */
/*           Visit一次且仅一次。一旦Visit()失败,则操作失败。 */
/*           使用辅助队列Q和访问标志数组visited */
void BFSTraverse(MGraph G,Status(*Visit)(VertexType))
{
	int i;
	LinkQueue q;
	int qe;
	for (i = 0; i < MAX_VERTEX_NUM; i++)
		visited[i] = FALSE;
	InitQueue(&q);

	if (visited[0] == FALSE) {
		visited[0] = TRUE;
		VisitFunc(G.vexs[0]);
		EnQueue(&q, 0);
	}

	while (!QueueEmpty(q)) {
		DeQueue(&q, &qe);
		for (i = FirstAdjVex(G, G.vexs[qe]); i >= 0; i = NextAdjVex(G, G.vexs[qe], G.vexs[i])) {
			if (visited[i] == FALSE) {
				visited[i] = TRUE;
				VisitFunc(G.vexs[i]);
				EnQueue(&q, i);
			}
		}
	}
}

/* 输出邻接矩阵G */
// copy from b7-1.c
void Display(MGraph G)
{
	int i,j;
	char s[7],s1[3];
	switch(G.kind)
	{
	case DG: strcpy(s,"有向图\0");
		strcpy(s1,"弧\0");
		break;
	case DN: strcpy(s,"有向网\0");
		strcpy(s1,"弧\0");
		break;
	case AG: strcpy(s,"无向图\0");
		strcpy(s1,"边\0");
		break;
	case AN: strcpy(s,"无向网\0");
		strcpy(s1,"边\0");
	}
	printf("%d个顶点%d条%s的%s\n",G.vexnum,G.arcnum,s1,s);
	printf("sizeof(struct MGraph):%d, sizeof(struct ArcCell):%d, sizeof(AdjMatrix):%d\n", sizeof(MGraph), sizeof(ArcCell), sizeof(AdjMatrix));
	for(i=0;i<G.vexnum;++i) /* 输出G.vexs */
		printf("G.vexs[%d]=%s\n",i,G.vexs[i]);
	printf("G.arcs.adj:\n"); /* 输出G.arcs.adj */
	for(i=0;i<G.vexnum;i++)
	{
		for(j=0;j<G.vexnum;j++)
			printf("%6d",G.arcs[i][j].adj);
		printf("\n");
	}
	printf("G.arcs.info:\n"); /* 输出G.arcs.info */
	printf("顶点1(弧尾) 顶点2(弧头) 该%s信息：\n",s1);
	if(G.kind<2) /* 有向 */
		for(i=0;i<G.vexnum;i++)
			for(j=0;j<G.vexnum;j++)
			{
				if(G.arcs[i][j].info)
					printf("%5s %11s     %s\n",G.vexs[i],G.vexs[j],G.arcs[i][j].info);
			}
	else /* 无向 */
	{
		for(i=0;i<G.vexnum;i++)
			for(j=i+1;j<G.vexnum;j++)
				if(G.arcs[i][j].info)
					printf("%5s %11s     %s\n",G.vexs[i],G.vexs[j],G.arcs[i][j].info);
	}
	printf("done with display\n");
}
