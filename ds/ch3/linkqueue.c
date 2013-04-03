/* 链队列(存储结构由linkqueue.h定义)的基本操作(9个) */
/* #include "../include/common.h" */
/* typedef int QElemType; */
/* typedef struct QNode { */
/* 	QElemType data; */
/* 	struct QNode *next; */
/* } QNode, *QueuePtr; */

/* typedef struct { */
/* 	QueuePtr front, rear; */
/* } LinkQueue; */

/* 构造一个空队列Q */
Status InitQueue(LinkQueue *q)
{
	q->front = (QueuePtr)malloc(sizeof(struct QNode));
	if (!q->front)
		return ERROR;
	q->front->next = NULL;
	q->rear = q->front;
	return OK;
}

/* 销毁队列Q(无论空否均可) */
Status DestroyQueue(LinkQueue *q)
{
	while (q->front) {
		q->rear = q->front->next;
		free(q->front);
		q->front = q->rear;
	}
	return OK;
}

/* 将Q清为空队列 */
Status ClearQueue(LinkQueue *q)
{
	QueuePtr p1, p2;

	q->rear = q->front;
	q->front->next = NULL;
	p1 = q->front->next;

	while (p1) {
		p2 = p1->next;
		free(p1);
		p1 = p2;
	}

	return OK;
}

/* 若Q为空队列,则返回TRUE,否则返回FALSE */
Status QueueEmpty(LinkQueue q)
{
	if (q.rear == q.front)
		return TRUE;
	else
		return FALSE;
}

/* 求队列的长度 */
int QueueLength(LinkQueue q)
{
	int i = 0;
	QueuePtr ptr;

	ptr = q.front;
	while (ptr != q.rear) {
		i++;
		ptr = ptr->next;
	}
	return i;
}

/* 若队列不空,则用e返回Q的队头元素,并返回OK,否则返回ERROR */
Status GetHead_Q(LinkQueue q, QElemType *e)
{
	if (q.front != q.rear) {
		*e = q.front->next->data;
		return OK;
	} else
		return ERROR;
}

/* 插入元素e为Q的新的队尾元素 */
Status EnQueue(LinkQueue *q, QElemType e)
{
	QueuePtr ptr = (QueuePtr)malloc(sizeof(struct QNode));
	if (ptr) {
		ptr->data = e;
		ptr->next = NULL;
		q->rear->next = ptr;
		q->rear = ptr;
		return OK;
	} else
		return ERROR;
}

/* 若队列不空,删除Q的队头元素,用e返回其值,并返回OK,否则返回ERROR */
Status DeQueue(LinkQueue *q, QElemType *e)
{
	QueuePtr ptr;
	if (q->front == q->rear)
		return ERROR;
	ptr = q->front->next;
	*e = ptr->data;
	q->front->next = ptr->next;
	if (ptr == q->rear)
		q->rear = q->front;
	free(ptr);
	return OK;
}

/* 从队头到队尾依次对队列Q中每个元素调用函数vi()。一旦vi失败,则操作失败 */
Status QueueTraverse(LinkQueue q, void (* vi)(QElemType))
{
	QueuePtr ptr;
	ptr = q.front->next;

	while (ptr) {
		vi(ptr->data);
		ptr = ptr->next;
	}
	printf("\n");
	return OK;
}
