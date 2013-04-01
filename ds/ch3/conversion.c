/* 调用算法3.1的程序 */
/* 数值转换*/
#include "../include/common.h"
typedef int SElemType; /* 定义栈元素类型，此句要在c3-1.h的前面 */
#include "sqstack.h"
#include "sqstack.c"

/* 对于输入的任意一个非负十进制整数，打印输出与其等值的八进制数 */
Status conversion()
{
  SqStack s;
  int n;

  if (InitStack(&s) != OK)
    return ERROR;
  printf("input a decimal number: ");
  scanf("%d",&n);

  if (n < 0) {
    printf("invalid number\n");
    return ERROR;
  }
  
  while (n) {
    Push(&s, n%8);
    n = n/8;
  }

  printf("after conversion: ");
  while (!StackEmpty(s)) {
    Pop(&s, &n);
    printf("%d", n);
  }
  printf("\n");

  DestroyStack(&s);
  return OK;
  
}

void main()
{
  conversion();
}


