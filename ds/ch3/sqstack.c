Status InitStack(SqStack *s);
Status DestroyStack(SqStack *s);
Status ClearStack(SqStack *s);
Status StackEmpty(SqStack s);
int StackLength(SqStack s);
Status GetTop(SqStack s, SElemType *e);
Status Push(SqStack *s, SElemType e);
Status Pop(SqStack *s, SElemType *e);
Status StackTraverse(SqStack s, Status (* visit)(SElemType));

/* 构造一个空栈S */
Status InitStack(SqStack *s)
{
	s->base = (SElemType *)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	if (s->base == NULL)
		return ERROR;
	s->top = s->base;
	s->stacksize = STACK_INIT_SIZE;
	return OK;
}

/* 销毁栈S，S不再存在 */
Status DestroyStack(SqStack *s)
{
	free(s->base);
	s->base = s->top = NULL;
	s->stacksize = 0;
	return OK;
}

/* 把S置为空栈 */
Status ClearStack(SqStack *s)
{
	s->top = s->base;
	return OK;
}

/* 若栈S为空栈，则返回TRUE，否则返回FALSE */
Status StackEmpty(SqStack s)
{
	if (s.base == s.top)
		return TRUE;
	else
		return FALSE;
}

/* 返回S的元素个数，即栈的长度 */
int StackLength(SqStack s)
{
	return s.top - s.base;
}

/* 若栈不空，则用e返回S的栈顶元素，并返回OK；否则返回ERROR */
Status GetTop(SqStack s, SElemType *e)
{
	if (s.top != s.base) {
		*e = *(s.top - 1);
		return OK;
	}
	return ERROR;
}

/* 插入元素e为新的栈顶元素 */
Status Push(SqStack *s, SElemType e)
{
	if (s->top - s->base >= s->stacksize) {
		s->base = (SElemType *)realloc(s->base, (s->stacksize + STACK_INCREMENT) * sizeof(SElemType));
		if (s->base == NULL)
			return ERROR;
		s->top = s->base + s->stacksize;
		s->stacksize += STACK_INCREMENT;
	}

	*(s->top) = e;
	s->top++;
	return OK;
}

/* 若栈不空，则删除S的栈顶元素，用e返回其值，并返回OK；否则返回ERROR */
Status Pop(SqStack *s, SElemType *e)
{
	if (s->top != s->base) {
		*e = *--(s->top);
		return OK;
	}
	return ERROR;
}

/* 从栈底到栈顶依次对栈中每个元素调用函数visit()。 */
/* 一旦visit()失败，则操作失败 */
Status StackTraverse(SqStack s, Status (* visit)(SElemType))
{
	SElemType *p;
	p = s.base;

	while (p != s.top) {
		visit(*p);
		p++;
	}
	return OK;
}
