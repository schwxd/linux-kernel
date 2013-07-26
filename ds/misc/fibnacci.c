#include <stdio.h>
#include <stdlib.h>

int fib(int n)
{
	int unused;
	if (n == 0)
		return 0;
	else if (n == 1)
		return 1;
	else if (n > 1)
		return (fib(n-1) + fib(n-2));	
	return 0;
}
int main(int argc, char** argv){
	printf("argc=%d\n", argc);
	int i;
	for(i=0;i<argc;i++)
		printf("%d argv: %s\n", argc, *(argv + i));

	if (argc==2)
		i = atoi(*(argv+1));

	printf("args converted: %d\n", i);
	printf("fib(%d)=%d\n", i, fib(i));		
	return 0;
}
