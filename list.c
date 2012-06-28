#include <stdio.h>
#include <stdlib.h>

typedef int ElemType;

struct LIST {
	int index;
	ElemType a;
	ElemType b;
	ElemType c;
	struct LIST *next;
};

int initList(struct LIST *list)
{
	list = (struct LIST *)malloc(sizeof(struct LIST));
	if(!list) {
		printf("no enough memory.\n");
		return -1;
	}

	return 0;
}

int main()
{
	struct LIST *list;
	int ret;

	ret = initList(list);
	if(ret)
		printf("initList failed\n");



	return 0;
}
