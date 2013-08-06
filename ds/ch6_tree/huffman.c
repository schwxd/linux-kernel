#include "../include/common.h"


/* 赫夫曼树和赫夫曼编码的存储表示 */
typedef struct {
	 unsigned int weight;
	 unsigned int parent, lchild, rchild;
} HTNode, *HuffmanTree;		/* 动态分配数组存储赫夫曼树 */

typedef char ** HuffmanCode;	/* 动态分配数组存储赫夫曼编码表 */


 int min1(HuffmanTree t,int i)
 { /* 函数void select()调用 */
   int j,flag;
   unsigned int k=1000; /* 取k为不小于可能的值 */
   for(j=0;j<i;j++)
	   if(t[j].weight < k &&t[j].parent == 0) {
		   k=t[j].weight;
		   flag=j;
	   }
   t[flag].parent=1;
   return flag;
 }

/* s1为最小的两个值中序号小的那个 */
void select1(HuffmanTree t,int i,int *s1,int *s2)
{ 
	int j;
	*s1 = min1(t,i);
	*s2 = min1(t,i);

	if(*s1 > *s2) {
		j = *s1;
		*s1 = *s2;
		*s2 = j;
	}
}


/* w存放n个字符的权值(均>0),构造赫夫曼树HT,并求出n个字符的赫夫曼编码HC */
/* 算法6.12 */
void HuffmanCoding(HuffmanTree *HT,HuffmanCode *HC,int *w,int n) 
{
	int i, idx, j, total, par;
	int s1, s2;

	total = 2 * n - 1;
	*HT = (HuffmanTree)malloc(total * sizeof(HTNode));
	if (!*HT)
		exit(ERROR);

	for (i = 0; i < n; i++) {
		(*HT)[i].weight = w[i];
		(*HT)[i].parent = 0;
		(*HT)[i].lchild = 0;
		(*HT)[i].rchild = 0;

		/* p->weight = *w++; */
		/* p->parent = 0; */
		/* p->lchild = 0; */
		/* p->rchild = 0; */
	}

	for (; i < total; i++) {
		(*HT)[i].weight = 0;
		(*HT)[i].parent = 0;
		(*HT)[i].lchild = 0;
		(*HT)[i].rchild = 0;		
	}

	// print out all the nodes
	for (i = 0; i< total; i++) {
//		p = HT[i];
//		printf("node %d: weight:%d, parent:%d, lchild:%d, rchild:%d\n", i, p->weight, p->parent, p->lchild, p->rchild);
		printf("node %d: weight:%d, parent:%d, lchild:%d, rchild:%d\n", i, (*HT)[i].weight, (*HT)[i].parent, (*HT)[i].lchild, (*HT)[i].rchild);
	}


	for (i = n; i < total; i++) {
		select1(*HT, i, &s1, &s2);
		printf("select s1:%d, s2:%d\n", s1, s2);
		printf("HT[%d]->weight: %d, HT[%d]->weight: %d\n", s1, (*HT)[s1].weight, s2, (*HT)[s2].weight);

		(*HT)[i].weight = (*HT)[s1].weight + (*HT)[s2].weight;
		(*HT)[i].lchild = s1;
		(*HT)[i].rchild = s2;
		(*HT)[s1].parent = i;
		(*HT)[s2].parent = i;
	}

	for (i = 0; i< total; i++) {
//		p = HT[i];
//		printf("node %d: weight:%d, parent:%d, lchild:%d, rchild:%d\n", i, p->weight, p->parent, p->lchild, p->rchild);
		printf("node %d: weight:%d, parent:%d, lchild:%d, rchild:%d\n", i, (*HT)[i].weight, (*HT)[i].parent, (*HT)[i].lchild, (*HT)[i].rchild);
	}

	*HC = (char **)malloc(n * sizeof(char *));

		char *tmp = (char *)malloc(n * sizeof(char));
		if (!tmp)
			exit(ERROR);
		tmp[n-1] = '\0';

	for (i = 0; i < n; ++i) {
		for (j = i, idx = n - 1, par = (*HT)[j].parent; par != 0; ) {
			if ((*HT)[par].lchild == j)
				tmp[--idx] = '0';
			else
				tmp[--idx] = '1';

			j = par;
			par = (*HT)[j].parent;
		}
		

		/* for (idx = (*HT)[i].parent, j = 1; idx != 0; j = (*HT)[idx].parent, j++) { */
		/* 	if ((*HT)[j].lchild == (*HT)[i].weight) */
		/* 		tmp[n-1-j] = 0; */
		/* 	else */
		/* 		tmp[n-1-j] = 1; */
		/* } */

		printf("No. %d 's coding length is %d\n", i, idx);

		/* char *code = (char *)malloc((idx + 1) * sizeof(char)); */
		/* strncpy(code, &tmp[n-1-idx], idx+1); */
		/* puts(tmp); */
		/* (*HC)[i] = code; */

		(*HC)[i] = (char *)malloc((n-idx) * sizeof(char));
//		strncpy((*HC)[i], &tmp[n-1-idx], idx+1);
		strcpy((*HC)[i], &tmp[idx]);
		puts((*HC)[i]);
		

		/* q = (p+i); */
		/* j = 1; */
		/* while (q->parent != 0) { */
		/* 	if (q->parent->lchild == q->weight) */
		/* 		code[n-i-j] = 0; */
		/* 	else */
		/* 		code[n-i-j] = 1; */
		/* 	j++; */
		/* 	q = q->parent; */
		/* } */
		/* (*HC + i) = &code[n-i-j]; */
	}
	/* for (i = 0; i < n; ++i) { */
	/* 	printf("HC[i]: %s\n", *HC[i]);		 */
	/* } */
	

}

 int main()
 {
   HuffmanTree HT;
   HuffmanCode HC;
   int *w, n,i;

   printf("请输入权值的个数(>1)：");
   scanf("%d",&n);
   w=(int*)malloc(n*sizeof(int));
   printf("请依次输入%d个权值(整型)：\n",n);
   for(i=0;i<=n-1;i++)
     scanf("%d",w+i);
   for(i=0;i<=n-1;i++)
	   printf("No. %d 's weight:%d\n", i, *(w+i));
   HuffmanCoding(&HT,&HC,w,n);

   for(i=0;i<n;i++)
     puts(HC[i]);

   return 0;
 }
