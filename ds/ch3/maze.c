/* 利用栈求解迷宫问题（只输出一个解，算法3.3） */
#include "../include/common.h"

/* 迷宫坐标位置类型 */
typedef struct {
	int x;
	int y;
} PosType;

/* 迷宫的最大行列 */
#define MAXLENGTH 20

/* 迷宫数组 */
int m[MAXLENGTH][MAXLENGTH];
int curstep = 1; /* 当前足迹 */

typedef struct {
	int ord;	//通道块在路径上的序号
	PosType seat;	//通道块在迷宫中的坐标位置
	int di;		//从此通道块走向下一通道块的方向（0-3表示东-北）
} SElemType;

#include "sqstack.h"
#include "sqstack.c"

/* 定义墙元素值为0,可通过路径为1,不能通过路径为-1,通过路径为足迹 */

/* 当迷宫m的p点的序号为1(可通过路径)，return OK; 否则，return ERROR。 */
Status Pass(PosType p)
{
	if (m[p.x][p.y] == 1)
		return OK;
	else
		return ERROR;
}

/* 使迷宫m的p点的序号变为足迹(curstep) */
void FootPrint(PosType p)
{
	m[p.x][p.y] = curstep;
}

/* 根据当前位置及移动方向，返回下一位置 */
PosType NextPos(PosType p, int di)
{
	PosType direct[4] = {{0,1}, {1,0}, {0.-1}, {-1,0}};
	PosType c;

	/* 移动方向,依次为东南西北 */
	c.x = p.x + direct[di].x;
	c.y = p.y + direct[di].y;

	return c;
}

/* 使迷宫m的b点的序号变为-1(不能通过的路径) */
void MarkPrint(PosType p)
{
	m[p.x][p.y] = -1;
}

/* 若迷宫maze中存在从入口start到出口end的通道，则求得一条 */
/* 存放在栈中(从栈底到栈顶)，并返回TRUE；否则返回FALSE */
Status MazePath(PosType start, PosType end)
{
	SqStack s;
	PosType curpos;
	SElemType e;
	
	InitStack(&s);
	curpos = start;
	do {
		if (Pass(curpos)) {
			FootPrint(curpos);
			e.ord = curstep;
			e.seat.x = curpos.x;
			e.seat.y = curpos.y;
			e.di = 0;
			Push(&s, e);
			curstep++;
			if (curpos.x == end.x && curpos.y == end.y)
				return OK;
			curpos = NextPos(curpos, e.di);
		} else {
			if (!StackEmpty(s)) {
				Pop(&s, &e);
				curstep--;
				while (e.di == 3 && !StackEmpty(s)) {
					MarkPrint(e.seat);
					Pop(&s, &e);
					curstep--;
				}
				if (e.di < 3) {
					e.di++;
					Push(&s, e);
					curstep++;
					curpos = NextPos(e.seat, e.di);
				}
			} 
		}
		
	} while (!StackEmpty(s));

	return FALSE;
}

 void Print(int x,int y)
 { /* 输出迷宫的解 */
   int i,j;
   for(i=0;i<x;i++)
   {
     for(j=0;j<y;j++)
       printf("%3d",m[i][j]);
     printf("\n");
   }
 }

 void main()
 {
   PosType begin,end;
   int i,j,x,y,x1,y1;
   printf("请输入迷宫的行数,列数(包括外墙)：");
   scanf("%d,%d",&x,&y);
   for(i=0;i<x;i++) /* 定义周边值为0(同墙) */
   {
     m[0][i]=0; /* 行周边 */
     m[x-1][i]=0;
   }
   for(j=1;j<y-1;j++)
   {
     m[j][0]=0; /* 列周边 */
     m[j][y-1]=0;
   }
   for(i=1;i<x-1;i++)
     for(j=1;j<y-1;j++)
       m[i][j]=1; /* 定义通道初值为1 */
   printf("请输入迷宫内墙单元数：");
   scanf("%d",&j);
   printf("请依次输入迷宫内墙每个单元的行数,列数：\n");
   for(i=1;i<=j;i++)
   {
     scanf("%d,%d",&x1,&y1);
     m[x1][y1]=0; /* 定义墙的值为0 */
   }
   printf("迷宫结构如下:\n");
   Print(x,y);
   printf("请输入起点的行数,列数：");
   scanf("%d,%d",&begin.x,&begin.y);
   printf("请输入终点的行数,列数：");
   scanf("%d,%d",&end.x,&end.y);
   if(MazePath(begin,end)) /* 求得一条通路 */
   {
     printf("此迷宫从入口到出口的一条路径如下:\n");
     Print(x,y); /* 输出此通路 */
   }
   else
     printf("此迷宫没有从入口到出口的路径\n");
 }

