#include <stdio.h>

void fa(int n)
{
	n = 10;
	printf("n in fa(): %d\n", n);
}

void fb(int *n)
{
	*n = 100;
	printf("n in fb(): %d\n", *n);
}

int main(int argc, char ** argv)
{
	int n;
	n = 8;
	printf("before fa: n in main=%d\n", n);
	fa(n);
	printf("after fa, before fb: n in main=%d\n", n);
	fb(&n);
	printf("after fb: n in main=%d\n", n);
	return 0;
}
