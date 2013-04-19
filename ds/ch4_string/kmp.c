#include "sstring.c"

int get_next(SString t, int next[])
{
	int i, j;

	i = 1;
	j = 0;
	next[1] = 0;

	while (i < t[0]) {
		if (j == 0 || t[i] == t[j]) {
			++i;
			++j;
			next[i] = j;
		} else
			j = next[j];
	}
	
}

int Index_KMP(SString s, SString t, int pos, int next[])
{
	int i, j;
	i = pos;
	j = 1;

	while (i <= s[0] && j <= t[0]) {
		if (j == 0 || s[i] == t[j]) {
			i++;
			j++;
		} else {
			j = next[j];
		}
	}

	if ( j > t[0])
		return i-t[0];
	else
		return 0;
}


void main()
{
	int i,j,*p;
	SString s1,s2; /* 以教科书中图4.5为例 */
	StrAssign(s1,"acabaabaabcacaabc");
	printf("主串为: ");
	StrPrint(s1);
	StrAssign(s2,"abaabcac");
	printf("子串为: ");
	StrPrint(s2);
	i=StrLength(s2);
	p=(int*)malloc((i+1)*sizeof(int)); /* 生成s2的next数组 */
	get_next(s2,p);
	printf("子串的next函数为: ");
	for(j=1;j<=i;j++)
		printf("%d ",*(p+j));
	printf("\n");
	i=Index_KMP(s1,s2,1,p);
	if(i)
		printf("主串和子串在第%d个字符处首次匹配\n",i);
	else
		printf("主串和子串匹配不成功\n");
}
