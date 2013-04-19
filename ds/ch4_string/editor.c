#include "hstring.c"
#define MAX_LEN 25 /* 文件最大行数 */
#define LINE_LEN 75 /* 每行字符数最大值+1 */
#define NAME_LEN 20 /* 文件名最大长度(包括盘符、路径)+1 */

/* 全局变量 */
HString T[MAX_LEN];
char str[LINE_LEN],filename[NAME_LEN]="";
FILE *fp;
int n=0; /* 文件行数 */

void Open()
{
	int i;

	if (fp != NULL) {
		printf("there is already one opend file\n");
		return;
	}

	printf("input file path and name: ");
	scanf("%s", filename);
	fp = fopen(filename, "r");
	if (!fp) {
		printf("open file %s error\n", str);
		return;
	}

	fgets(str, LINE_LEN, fp);
	i = strlen(str);

	while (i > 0) {
		printf("the No. %d line has %d charaters: %s\n", n, i, str);
		if (str == NULL) {
			printf("get eof\n");
		}
		
		StrAssign(&T[n], str);
		n++;
		if (n > MAX_LEN) {
			printf("too many lines in the file\n");
			fclose(fp);
			return;
		}
		fgets(str, LINE_LEN, fp);
		i = strlen(str);
	}
	fclose(fp);
}

void List()
{
	int i = 0;
	while (i < n) {
		printf("LINE %d: ", i);
		printf("%s\n", T[i].ch);
	}
}

void Insert()
{
	int l, m;
	int i;

	printf("input lines input and line No. m insert to :");
	scanf("%d,%d", &l, &m);

	if (l+m>MAX_LEN || l <= 0) {
		printf("invalid insert lines\n");
		return;
	}

	for (i=n;i>=l;i--) {
		T[i+m] = T[i];
	}

	printf("input the lines: \n");
	while (m > 0) {
//		fgets(str, fp);
//		scanf("%s", str);
		gets(str);
		ClearString(&T[l]);
		StrAssign(&T[l], str);
		l++;
	}
	n+=m;
}

void Delete()
{
	int l, m;
	int i;

	printf("input delete from line l, and totol line number m: ");
	scanf("%d,%d", &l, &m);

	if (l < 1 || l + m > n) {
		printf("invalid delete number\n");
		return;
	}

	for (i = l + m; i < n; ++i) {
		T[i-m] = T[i];
	}

	for (i = n - m; i < n; i++)
		ClearString(&T[i]);

	n -= m;
}

void Save()
{
	int i = 0;

	fp = fopen(filename, "w");
	if (!fp) {
		printf("save: open file filed\n");
		return;
	}

	while (i < n) {
		fputs(T[i].ch, fp);
		i++;
	}

	fclose(fp);
	
}

int main(int argc, char *argv[])
{
	Status s = TRUE;
	int i,k;
	for(i=0;i<MAX_LEN;i++) /* 初始化串 */
		InitString(&T[i]);
	while (s) {
		printf("请选择: 1.打开文件(新或旧)  2.显示文件内容\n");
		printf("        3.插入行  4.删除行  5.拷贝行  6.修改行\n");
		printf("        7.查找字符串        8.替换字符串\n");
		printf("        9.存盘退出          0.放弃编辑\n");
		scanf("%d",&k);
		switch (k)
		{
		case 1: Open();
			break;
		case 2: List();
			break;
		case 3: Insert();
			break;
		case 4: Delete();
			break;
		/* case 5: Copy(); */
		/* 	break; */
		/* case 6: Modify(); */
		/* 	break; */
		/* case 7: Search(); */
		/* 	break; */
		/* case 8: Replace1(); */
		/* 	break; */
		case 9: Save();
		case 0: s=FALSE;
		}
	}

	return 0;
}

