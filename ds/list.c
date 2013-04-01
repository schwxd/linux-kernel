#include <stdio.h>
#include <stdlib.h>

typedef int ElemType;

struct LIST {
	int index;
	ElemType a;
	ElemType b;
	ElemType c;
//	struct LIST *next;
};

int initList(struct LIST **list)
{
	*list = (struct LIST *)malloc(sizeof(struct LIST));
	if(!*list) {
		printf("no enough memory.\n");
		return -1;
	}

	printf("%s: list's addr:%p\n", __func__, list);
	printf("%s: list's value:%p\n", __func__, *list);
	return 0;
}

void destroyList(struct LIST *list)
{
	free((void*)list);
	list = NULL;
}

int main()
{
	struct LIST *list;
	int ret;

	list = NULL;
	printf("the pointer list's addr:%p\n", &list);
	printf("the pointer list's value after NULL:%p\n", list);

	ret = initList(&list);
	if(!ret) {
		printf("the pointer list's addr:%p\n", &list);
		printf("the pointer list's value after init:%p\n", list);
		destroyList(list);
	}

	return 0;
}
