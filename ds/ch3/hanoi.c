/* Hanoi塔问题,调用算法3.5的程序 */
#include<stdio.h>

/* 全局变量，搬动次数 */
int count = 0;

/* 第n个圆盘从塔座x搬到塔座z */
void move(char x, int n, char z)
{
//	printf("move plate %d from %c to %c\n", n, x, z);
	printf("第%i步: 将%i号盘从%c移到%c\n",++count, n, x, z);
}

/* 将塔座x上按直径由小到大且自上而下编号为1至n的n个圆盘 */
/* 按规则搬到塔座z上。y可用作辅助塔座 */
void Hanoi(int n, char x, char y, char z)
{
	if (n == 1)
		move(x, 1, z);
	else {
		Hanoi(n-1, x, z, y);
		move(x, n, z);
		Hanoi(n-1, y, x, z);
	}
}

int main(int argc, char *argv[])
{
	int n;
	printf("3个塔座为a、b、c，圆盘最初在a座，借助b座移到c座。请输入圆盘数：");
	scanf("%d", &n);
	Hanoi(n,'a', 'b', 'c');
	return 0;
}

