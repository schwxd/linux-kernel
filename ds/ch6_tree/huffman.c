#include "../include/common.h"


/* 赫夫曼树和赫夫曼编码的存储表示 */
typedef struct {
	unsigned int weight;
	unsigned int parent, lchild, rchild;
} HTNode, *HuffmanTree;		/* 动态分配数组存储赫夫曼树 */

typedef char ** HuffmanCode;	/* 动态分配数组存储赫夫曼编码表 */

/* w存放n个字符的权值(均>0),构造赫夫曼树HT,并求出n个字符的赫夫曼编码HC */
/* 算法6.12 */
void HuffmanCoding(HuffmanTree *HT,HuffmanCode *HC,int *w,int n) 
{
	int i, total;
	HuffmanTree p, s1, s2;

	total = 2 * n - 1;
	*HT = (HuffmanTree *)malloc(total * sizeof(HTNode));
	if (!*HT)
		return ERROR;

	for (i = 0, p = *HT; i < n; i++, p++) {
		p->weight = *w++;
		p->parent = 0;
		p->lchild = 0;
		p->rchild = 0;
	}

	for (; i < total; i++, p++) {
		p->weight = 0;
		p->parent = 0;
		p->lchild = 0;
		p->rchild = 0;		
	}

	for (i = n; i < total; i++) {
//		select(HT, n, s1, s2);
		(p+i)->weight = s1->weight + s2->weight;
		(p+i)->lchild = s1->weight;
		(p+i)->rchild = s2->weight;
		s1->parent = (p+i)->weight;
		s2->parent = (p+i)->weight;
	}

	HC = 

}

