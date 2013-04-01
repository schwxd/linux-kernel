/* 单链队列－－队列的链式存储结构 */
typedef struct QNode {
	QElemType data;
	struct QNode *next;
} QNode, *QueuePtr;

typedef struct {
	QueuePtr front, rear;
} LinkQueue;
