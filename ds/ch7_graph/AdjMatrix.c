typedef int VRType;
typedef char* VertexType;
typedef char InfoType;
#define MAX_INFO 10
#define INFINITE -1
#include "../include/common.h"

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
	char *filename;
	FILE *graphfile;
	VertexType va, vb;

	printf("input the graph data file (f7-1.dat)");
	scanf("%s", filename);
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
		fscanf(graphfile, "%c %c", va, vb);
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
	free((*G).vexs);
	free(
}

VertexType* GetVex(MGraph G,int v)
{ /* 初始条件: 图G存在，v是G中某个顶点的序号。操作结果: 返回v的值 */
}
Status PutVex(MGraph *G,VertexType v,VertexType value)
{ /* 初始条件: 图G存在，v是G中某个顶点。操作结果: 对v赋新值value */
}

int FirstAdjVex(MGraph G,VertexType v)
{ /* 初始条件: 图G存在,v是G中某个顶点 */
  /* 操作结果: 返回v的第一个邻接顶点的序号。若顶点在G中没有邻接顶点,则返回-1 */
}

int NextAdjVex(MGraph G,VertexType v,VertexType w)
{ /* 初始条件: 图G存在,v是G中某个顶点,w是v的邻接顶点 */
  /* 操作结果: 返回v的(相对于w的)下一个邻接顶点的序号, */
  /*	       若w是v的最后一个邻接顶点,则返回-1 */
}

void InsertVex(MGraph *G,VertexType v)
{ /* 初始条件: 图G存在,v和图G中顶点有相同特征 */
  /* 操作结果: 在图G中增添新顶点v(不增添与顶点相关的弧,留待InsertArc()去做) */
}

Status DeleteVex(MGraph *G,VertexType v)
{ /* 初始条件: 图G存在,v是G中某个顶点。操作结果: 删除G中顶点v及其相关的弧 */
}

Status InsertArc(ALGraph *G,VertexType v,VertexType w)
{ /* 初始条件: 图G存在,v和w是G中两个顶点 */
  /* 操作结果: 在G中增添弧<v,w>,若G是无向的,则还增添对称弧<w,v> */
}

Status DeleteArc(MGraph *G,VertexType v,VertexType w)
{ /* 初始条件: 图G存在,v和w是G中两个顶点 */
  /* 操作结果: 在G中删除弧<v,w>,若G是无向的,则还删除对称弧<w,v> */
}

void DFS(ALGraph G,int v)
{ /* 从第v个顶点出发递归地深度优先遍历图G。算法7.5 */
}

void DFSTraverse(ALGraph G,void(*Visit)(char*))
{ /* 对图G作深度优先遍历。算法7.4 */
}

void BFSTraverse(ALGraph G,void(*Visit)(char*))
{/*按广度优先非递归遍历图G。使用辅助队列Q和访问标志数组visited。算法7.6 */
}

void Display(ALGraph G)
{ /* 输出图的邻接矩阵G */
}
