 /*  括号匹配的检验，（限于()、[]） */
#include "../include/common.h"
typedef char SElemType; /* 定义栈元素类型，此句要在c3-1.h的前面 */
#include "sqstack.h"
#include "sqstack.c"

Status match()
{
  int m, n;
  char c;
  char str[50];
  SqStack s;

  printf("input ()[]: ");
  scanf("%s", str);

  printf("str: %s\n", str);

  InitStack(&s);
  n = 0;
  m = strlen(str);
  while (n < m) {
    switch (str[n]) {
    case '[':
    case '(':
      Push(&s, str[n]);
      n++;
      break;
    case ']':
    case ')':
      if (!StackEmpty(s)) {
	Pop(&s, &c);
	if ((c == '[' && str[n] == ']') || (c == '(' && str[n] == ')')) {
	  n++;
	  break;
	} else {
	  printf("ERROR the No.%d charater does not match\n", n);
	  return ERROR;
	}
      } else {
	printf("ERROR 缺左括号\n");
	return ERROR;
      }
    default:
      n++;
      break;
    }
  }

  if (StackEmpty(s)) {
    printf("it matches\n");
    return OK;
  } else {
    printf("ERROR 缺右括号\n");
    return ERROR;
  }
}

void main()
{
  match();
}
