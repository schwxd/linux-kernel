/* 队列的顺序存储结构(可用于循环队列和非循环队列) */
#define MAXQSIZE 10
typedef struct {
	QElemType *base; /* 初始化的动态分配存储空间 */
	int front;
	int rear;
} SqQueue;
