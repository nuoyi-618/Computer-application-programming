#include<stdio.h>
#include<stdlib.h>
#include < assert.h >
#define BUFSIZE 100
long mem = 0;
long cmpnum = 0;
struct keyNum
{
	char key[100];//关键字
	struct keyNum* next;
};
/*****动态内存******/
void* bupt_malloc(size_t size) {
	if (size <= 0)
	{
		return NULL;
	}

	mem += size;
	return malloc(size);
}
/*****插入到哈希表******/
struct keyNum* insertHash(struct keyNum* head, char s[100])
{
	struct keyNum* p0, * p1,*p2, * temp;
	temp = (struct keyNum*)bupt_malloc(sizeof(struct keyNum));
	strcpy(temp->key, s);
	//temp->key = s;
	p1 = head;
	p0 = temp;//要插入的节点（值为m);
	if (head == NULL)//1，原来的链表为空，插入到head后
	{
		head = p0;
		p0->next = NULL;
	}
	else//原来的链表不为空
	{
		while ((p0->key > p1->key) && (p1->next != NULL))//移动到适当位置
		{
			p2 = p1;
			p1 = p1->next;
		}
		if (p0->key <= p1->key)
		{
			if (head == p1)head = p0;//2，插入到第一个节点之前
			else p2->next = p0;//3,插入到p2指向的节点之后
			p0->next = p1;

		}
		else//4,插入到结尾处
		{
			p1->next = p0;
			p0->next = NULL;

		}
	}

	return(head);
}
/*****查找链表head中是否存在m******/
int searchHash(struct keyNum* head, char m[100])
{

	struct keyNum* p;
	p = head;
	while (p != NULL) {
		if (bupt_strcmp(p->key, m) == 0) {
			return 1;
		}
		else p = p->next;
	}
	return 0;
}



/*****字符比较******/
int byte_cmp(char a, char b) {
	cmpnum++;
	return a - b;
}
/*****字符串对比******/
int bupt_strcmp(const char* str1, const char* str2)
{
	while (byte_cmp(*str1, *str2) == 0)
	{
		assert((str1 != NULL) && (str2 != NULL));
		if (*str1 == '\0')
			return 0;
		str1++;
		str2++;
	}
	return *str1 - *str2;
}

/*****哈希函数******/
unsigned int PJWHash(const char* str, unsigned int length)
{
	const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
	const unsigned int ThreeQuarters = (unsigned int)((BitsInUnsignedInt * 3) / 4);
	const unsigned int OneEighth = (unsigned int)(BitsInUnsignedInt / 8);
	const unsigned int HighBits =
		(unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
	unsigned int hash = 0;
	unsigned int test = 0;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = (hash << OneEighth) + (*str);

		if ((test = hash & HighBits) != 0)
		{
			hash = ((hash ^ (test >> ThreeQuarters))& (~HighBits));
		}
	}

	return hash;
}
void free_vector(struct keyNum* pf) {
	if (pf == NULL) return;
	else {
		pf = pf->next;
		free_vector(pf);
		free(pf);
	}
}
void main()
{


	/*******读取文件*******/

	FILE* patterns;
	FILE* word;
	int i, j, patterns_n = 0, words_n = 0;
	char buf[BUFSIZE], c;
	char str[100];
	//打开文件
	patterns = fopen("./patterns-127w.txt", "r");
	word = fopen("./words-98w.txt", "r");

	if (patterns == NULL || word == NULL)
	{
		printf("Unable to open this file!\n");
		exit(0);
	}
	//计算文件中数据的行数
	while (!feof(patterns))
	{
		c = fgetc(patterns);
		if (c == '\n')
			patterns_n++;
	}
//	printf("patterns文本长=%d\n", patterns_n);//总文本长
	while (!feof(word))
	{
		c = fgetc(word);
		if (c == '\n')
			words_n++;
	}
//	printf("words文本长=%d\n", words_n);//总文本长
	/*******词库保存到结构体******/
	rewind(patterns);//将指针重置到第一行
	struct keyNum** hash = (struct keyNum**)bupt_malloc(sizeof(struct keyNum*) * (patterns_n+1)); //创建一个结构体含有(n+1)个数据
	
	for (i = 1; i <=patterns_n; i++) {//初始化
		hash[i] = NULL;
	}
	int len, m;

	
	for (i = 1; i <= patterns_n; i++)
	{
		
		fgets(buf, BUFSIZE, patterns);    //一次读取一行
		sscanf(buf, "%s ", &str);
		len = strlen(buf);
		m = PJWHash(buf,len) % (patterns_n);//计算得到关键字的哈希值
		hash[m] = insertHash(hash[m], str);//将关键字k插入到哈希值为m的链表中
	}
	rewind(word);
	int flag=0,right=0;
	FILE* result;
	result = fopen("./result.txt", "w"); //建立一个文字文件只写
	for (i = 0; i < words_n; i++)
	{
		
		fgets(buf, BUFSIZE, word);    //一次读取一行
		sscanf(buf, "%s ", &str);
		len = strlen(buf);
		m = PJWHash(buf, len) % (patterns_n);//计算得到关键字的哈希值
		
		flag = searchHash(hash[m], str);
		if (flag == 1) {
			fprintf(result, "%syes\n", str);
			right++;
		}
		else {
			fprintf(result, "%sno\n", str);
		}
	}
	fprintf(result, "检索结构占用内存量%d   字符比较次数%d   words总数%d   成功检索总数%d", mem / 1024, cmpnum, words_n, right);
	printf("检索结构占用内存量%d   字符比较次数%d   words总数%d   成功检索总数%d", mem / 1024, cmpnum, words_n, right);

	for (int i = 1; i < patterns_n+1; i++) {
		struct keyNum* p = hash[i];
		free_vector(hash[i]);
	}
	
	free(hash);
	fclose(result);
	fclose(patterns);	//关闭文件
	fclose(word);	//关闭文件
	getchar();
	return 0;
}


