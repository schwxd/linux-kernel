#include <stdio.h>
#include <stdlib.h>

typedef int ElemType;

struct LNode{
	ElemType data;
	struct LNode *next;
};

struct LinkList{
	struct LNode *head, *tail;
	int len;
};

int makeNode(struct LNode **node, ElemType e)
{
	*node = (struct LNode *)malloc(sizeof(struct LNode));
	if(!*node) {
		printf("no enough mem\n");
		return -1;
	}
	(*node)->data = e;
	(*node)->next = NULL;
	return 0;
}

void freenode(struct LNode *node)
{
	free(node);
}


int initList(struct LinkList **list)
{
	*list = (struct LinkList *)malloc(sizeof(struct LinkList));
	if(!*list) {
		printf("no enough mem\n");
		return -1;
	}

	struct LNode *header = (struct LNode *)malloc(sizeof(struct LNode));
	if(header) {
		header->next = NULL;
		(*list)->head = (*list)->tail = header;
	} else {
		printf("no enough mem\n");
		return -1;
	}

	(*list)->len = 0;
	return 0;
}

int clearList(struct LinkList *list)
{
	struct LNode *node;
	while(list->head != list->tail) {
		node = list->head;
		list->head = node->next;
		free(node);
	}
	list->len = 0;
	
	return 0;
}


int insNode(struct LinkList *list, struct LNode *node)
{
	node->next = list->head->next;
	list->head->next = node;
	list->tail = list->tail->next;
	list->len++;

	return 0;

}

int removeNode(struct LinkList *list, struct LNode **node)
{
	if(list->len == 0)
		return -1;

	int count;

	*node = list->tail;
	list->len--;

	list->tail = list->head;
	count = list->len;
	while(count--) {
		list->tail = list->tail->next;
	}

	return 0;
}


int append(struct LinkList *list, struct LNode *node)
{
	list->tail = node;
	while(node->next) {
		list->tail = node->next;
		list->len++;
	}

	return 0;
}

int listLength(struct LinkList *list)
{
	return list->len;
}

int listEmpty(struct LinkList *list)
{
	return !(list->len);
}

int locatePos(struct LinkList *list, int pos, struct LNode **node)
{
	if(pos > list->len) {
		printf("pos %d out of list's reach.\n", pos);
		return -1;
	}
	
	while(pos--) {
		*node = list->head->next;
	}	

	return 0;
}

int main()
{
	int ret;
	struct LNode *n1, *tmp;
	if(makeNode(&n1, 111)) {
		printf("error make node\n");
		goto error;
	}
	printf("n1->data:%d\n", n1->data);

	struct LinkList *list;
	if(initList(&list)) {
		printf("initList failed\n");
		goto error;
	}

	printf("before insList. list->head->data:%d, list->tail->data:%d\n", list->head->data, list->tail->data);
	insNode(list, n1);
	printf("after insList. list->head->data:%d, list->tail->data:%d\n", list->head->data, list->tail->data);

	ret = removeNode(list, &tmp);
	if(ret) {
		printf("error remove node\n");
		goto error;
	} else {
		printf("the removed node's data:%d\n", tmp->data);
	}

	printf("test locatePos\n");
	tmp = NULL;
	n1->data = 222;
	printf("list's length=%d\n", listLength(list));
	insNode(list, n1);
	printf("list's length=%d\n", listLength(list));
	locatePos(list, 1, &tmp);
	if(tmp == NULL)
		printf("tmp still NULL\n");
	printf("the 1st node's data:%d\n", tmp->data);
	
	return 0;

error:
	freenode(n1);
	return -1;
}
