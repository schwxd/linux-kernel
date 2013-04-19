#include "../include/common.h"

/* 串的定长顺序存储表示 */
#define MAXSTRLEN 50
typedef char SString[MAXSTRLEN + 1];

/* 串的堆分配存储表示*/
typedef struct {
	char *ch;	/* 若是非空串,则按串长分配存储区,否则ch为NULL */
	int length;	/* 串长度 */
} HString;

/* 生成一个其值等于串常量chars的串T */
Status StrAssign(HString *t, char *chars);

/* 返回S的元素个数,称为串的长度 */
int StrLength(HString s);

/* 若S>T,则返回值>0;若S=T,则返回值=0;若S<T,则返回值<0 */
int StrCompare(HString s, HString t);

/* 将S清为空串 */
Status ClearString(HString *s);

/* 用T返回由S1和S2联接而成的新串 */
Status Concat(HString *t, HString s1, HString s2);

/* 用Sub返回串S的第pos个字符起长度为len的子串。 */
/* 其中,1≤pos≤StrLength(S)且0≤len≤StrLength(S)-pos+1 */
Status SubString(HString *sub, HString s, int pos, int len);

/* 初始条件:串S存在。操作结果: 由串S复制得串T */
Status StrCopy(HString *t, HString s);

/* 初始条件: 串S存在。操作结果: 若S为空串,则返回TRUE,否则返回FALSE */
Status StrEmpty(HString s);

/* 初始化(产生空串)字符串T。另加 */
Status InitString(HString *s);

/* T为非空串。若主串S中第pos个字符之后存在与T相等的子串, */
/* 则返回第一个这样的子串在S中的位置,否则返回0 */
int Index(HString s, HString t, int pos);


/////////////////////////////////////////////////////////////////////////////
Status StrAssign(HString *t, char *chars)
{
	int len, i;

	if (t->ch != NULL)
		free(t->ch);

	len = strlen(chars);
	if (len == 0) {
		t->ch = NULL;
		t->length = 0;
	} else {
		t->ch = (char *)malloc(len * sizeof(char));
		if (t->ch == NULL)
			exit(OVERFLOW);

		for (i=0;i<len;i++) {
			t->ch[i] = chars[i];
		}
		t->length = len;
	}

	return OK;
}

int StrLength(HString s)
{
	return s.length;
}

/* 若S>T,则返回值>0;若S=T,则返回值=0;若S<T,则返回值<0 */
int StrCompare(HString s, HString t)
{
	int i;

	for (i=0; i<s.length && i<t.length; i++) {
		if (s.ch[i] != t.ch[i])
			return s.ch[i] - t.ch[i];
	}
	return s.length - t.length;

}

/* 将S清为空串 */
Status ClearString(HString *s)
{
	if (s && s->ch) {
		free(s->ch);
		s->ch = NULL;
		s->length = 0;
	}
	return OK;
}

/* 用T返回由S1和S2联接而成的新串 */
Status Concat(HString *t, HString s1, HString s2)
{
	int i;

	if (t->ch)
		free(t->ch);
	t->ch = (char *)malloc(s1.length + s2.length);
	if (!t->ch)
		exit(OVERFLOW);
	t->length = s1.length + s2.length;
	for(i=0; i<s1.length; i++)
		t->ch[i] = s1.ch[i];
	for(i=0; i<s2.length; i++)
		t->ch[s1.length + i] = s2.ch[i];
	return OK;
}

/* 用Sub返回串S的第pos个字符起长度为len的子串。 */
/* 其中,1≤pos≤StrLength(S)且0≤len≤StrLength(S)-pos+1 */
Status SubString(HString *sub, HString s, int pos, int len)
{
	int i;

	if (pos < 1 || pos > s.length || len < 0 || len > s.length - pos + 1)
		return ERROR;

	if (sub->ch)
		free(sub->ch);
	sub->ch = (char *)malloc(len * sizeof(char));
	if (!sub->ch)
		exit(OVERFLOW);
	for (i=0;i<len;i++) {
		sub->ch[i] = s.ch[pos-1+i];
	}
	sub->length = len;
	return OK;
}

/* 初始条件:串S存在。操作结果: 由串S复制得串T */
Status StrCopy(HString *t, HString s)
{
	int i;

	if (t->ch)
		free(t->ch);
	t->ch = (char *)malloc(s.length * sizeof(char));
	if (!t->ch)
		exit(OVERFLOW);

	for (i=0;i<s.length;i++) {
		t->ch[i] = s.ch[i];
	}
	t->length = s.length;
	return OK;
}

/* 初始条件: 串S存在。操作结果: 若S为空串,则返回TRUE,否则返回FALSE */
Status StrEmpty(HString s)
{
	if (s.length == 0 && s.ch == NULL)
		return TRUE;
	else
		return FALSE;
}

/* 初始化(产生空串)字符串T。另加 */
Status InitString(HString *s)
{
	s->ch = NULL;
	s->length = 0;
	return OK;
}

/* 1≤pos≤StrLength(S)+1。在串S的第pos个字符之前插入串T */
Status StrInsert(HString *s, int pos, HString t)
{
	int i;

	if (pos < 1 || pos > s->length + 1)
		return ERROR;

	if (t.length == 0)
		return OK;

	s->ch = (char *)realloc(s->ch, (s->length + t.length) * sizeof(char));
	if (!s->ch)
		exit(OVERFLOW);

	/* for (i = s->length; i > pos; i--) */
	/* 	s->ch[i+t.length] = s->ch[i]; */
	/* for (i = 0; i < t.length; i++) */
	/* 	s->ch[i+pos] = t.ch[i]; */

	for (i = s->length-1; i >= pos-1; --i)
		s->ch[i+t.length] = s->ch[i];
	for (i = 0; i < t.length; i++)
		s->ch[i+pos-1] = t.ch[i];

	s->length += t.length;
	return OK;
}

/* 从串S中删除第pos个字符起长度为len的子串 */
Status StrDelete(HString *s, int pos, int len)
{
	int i;

	if (s->length < pos + len - 1)
		return ERROR;

	for (i=pos-1; i<= s->length - len; i++)
		s->ch[i] = s->ch[i+len];
	s->length -= len;
	s->ch = (char *)realloc(s->ch, s->length * sizeof(char));
	return OK;
}

/* 初始条件: 串S,T和V存在,T是非空串（此函数与串的存储结构无关） */
/* 操作结果: 用V替换主串S中出现的所有与T相等的不重叠的子串 */
Status StrReplace(HString *s, HString t, HString v)
{
	int i = 1;
	
	if (StrEmpty(t))
		return ERROR;
	do {
		i = Index(*s, t, i);
		if (i) {
			StrDelete(s, i, t.length);
			StrInsert(s, i, v);
			i += v.length;
		}
	} while (i);

	return OK;
}

/* T为非空串。若主串S中第pos个字符之后存在与T相等的子串, */
/* 则返回第一个这样的子串在S中的位置,否则返回0 */
int Index(HString s, HString t, int pos)
{
	int n, m, i;
	HString sub;
	InitString(&sub);

	if (pos > 0) {
		i = pos;
		n = s.length;
		m = t.length;
		while (i <= n-m+1) {
			SubString(&sub, s, i, m);
			if (StrCompare(sub, t) != 0)
				++i;
			else
				return i;
		}
	}
	return 0;
}

/* 输出T字符串。另加 */
void StrPrint(HString T)
{
	int i;
	for (i = 0; i < T.length; i++)
		printf("%c", T.ch[i]);
	printf("\n");
} 
