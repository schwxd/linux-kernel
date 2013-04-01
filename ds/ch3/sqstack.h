/* 栈的顺序存储表示 */
#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 10
typedef struct SqStackStruct {
	SElemType *base;
	SElemType *top;
	int stacksize;
} SqStack;
