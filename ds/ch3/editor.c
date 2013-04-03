/* 编辑器 */
#include "../include/common.h"
typedef char SElemType; /* 定义栈元素类型，此句要在c3-1.h的前面 */
#include "sqstack.h"
#include "sqstack.c"

FILE *fp;

Status sendToFile(SElemType c)
{
	fputc(c, fp);
	return OK;
}

void LineEdit()
{
	char ch, c;
	SqStack s;

	InitStack(&s);
	ch = getchar();

	while (ch != EOF) {
		while ((ch != EOF) && (ch != '\n')) {
			switch (ch) {
			case '#':
				Pop(&s, &c);
				break;
			case '@':
				ClearStack(&s);
				break;
			default:
				Push(&s, ch);
				if (ch == EOF)
					printf("\n push eof\n");
				break;
			}
			ch = getchar();
		}
		// '\n' detected, send out and clear whole stack 
		if (ch == '\n') {
			StackTraverse(s, sendToFile);
			fputc('\n', fp);
			ClearStack(&s);
			ch = getchar();
		} else if (ch == EOF) {
			StackTraverse(s, sendToFile);
			ClearStack(&s);
			printf(" now exit\n");
		}
		/* if (ch == '\n') */
		/* 	fputc('\n', fp); */
		/* if (ch != EOF) */
		/* 	ch = getchar(); */
		/* else */
		/* 	printf("ch == eof, should exit\n"); */
	}
	DestroyStack(&s);
}

int main(int argc, char *argv[])
{
	fp = fopen("/home/wangxd/editor.file", "w");
	if (fp) {
		LineEdit();
	}
	fclose(fp);
	return 0;
}

