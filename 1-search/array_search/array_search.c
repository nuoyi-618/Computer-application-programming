#include <stdio.h>
#include <stdlib.h>
#include < assert.h >
//文件操作 格式化读取 保存到结构体数组 
#define BUFSIZE 100

int mem = 0;
int cmpnum = 0;
struct dict
{
	char word[65];
};
int byte_cmp(char a, char b) { 
	cmpnum++; 
	return a - b; 
}
int bupt_strcmp(const char* str1, const char* str2)
{
	while (byte_cmp (*str1,*str2)==0)
	{
		assert((str1 != NULL) && (str2 != NULL));
		if (*str1 == '\0')
			return 0;
		str1++;
		str2++;
	}
	return *str1 - *str2;
}

void* bupt_malloc(size_t size) {
	if (size <= 0) 
	{ 
		return NULL;
	}
	
	mem += size; 
	return malloc(size);
}

void main()
{
	/*******读取文件*******/
	
	FILE* patterns;
	FILE* word;
	int i, j, patterns_n=0, words_n = 0;
	char buf[BUFSIZE], c;
	//打开文件
	patterns = fopen("./patterns-127w.txt", "r");
	word = fopen("./words-98w.txt", "r");

	if (patterns == NULL|| word == NULL)
	{
		printf("Unable to open this file!\n");
		exit(0);
	}
	/*
	//计算最长字符串长度
	int n=0;
	char s[100], t[100];
	while(!feof(patterns))
	{	fgets(s,100,patterns);
		if(strlen(s)>n){
			strcpy(t,s);
			n=strlen(s);
		}
	}	*/
	//计算文件中数据的行数
	while (!feof(patterns))
	{
		c = fgetc(patterns);
		if (c == '\n')
			patterns_n++;
	}
	//printf("patterns文本长=%d\n", patterns_n);//总文本长
	while (!feof(word))
	{
		c = fgetc(word);
		if (c == '\n')
			words_n++;
	}
	//printf("words文本长=%d\n", words_n);//总文本长
	/*******词库保存到结构体******/

	rewind(patterns);//将指针重置到第一行
	struct dict* dict_words = NULL;
	dict_words = (struct dict*)bupt_malloc(sizeof(struct dict) * (patterns_n + 1)); //创建一个结构体含有(n+1)个数据
	for (i = 1; i <= patterns_n; i++)
	{
		fgets(buf, BUFSIZE, patterns);    //一次读取一行
		sscanf(buf, "%s ", &dict_words[i].word);
	}

	fclose(patterns);	//关闭文件

	rewind(word);
	struct dict* test_words = NULL;
	test_words = (struct dict*)malloc(sizeof(struct dict) * (words_n + 1)); //创建一个结构体含有(n+1)个数据
	for (i = 1; i <= words_n; i++)
	{
		fgets(buf, BUFSIZE, word);    //一次读取一行
		sscanf(buf, "%s ", &test_words[i].word);
	}
	fclose(word);
	/********匹配并写文件***********/
	int rigth = 0;
	FILE* result;
	result = fopen("./result.txt", "w"); //建立一个文字文件只写
	int flag = 0;
	for (i = 1; i <= words_n; i++)
	{
		for (j = 1; j < patterns_n; j++)
		{
			if (strlen(test_words[i].word)== strlen(dict_words[j].word)) {
				if (bupt_strcmp(test_words[i].word, dict_words[j].word) == 0) {
					flag = 1;
					//printf("%s--%syes\n",test_words[i].word);
					rigth++;
				}
			}
		}
		if(flag==1){
			fprintf(result, "%syes\n", test_words[i].word);
		flag = 0;
		}
		else {
			fprintf(result, "%sno\n", test_words[i].word);
		}
	}

	fprintf(result,"检索结构占用内存量%d   字符比较次数%d   words总数%d   成功检索总数%d", mem / 1024, cmpnum, words_n, rigth);
	printf("检索结构占用内存量%d   字符比较次数%d   words总数%d   成功检索总数%d", mem / 1024, cmpnum, words_n, rigth);
	fclose(result);
	free(dict_words); //释放
	free(test_words); //释放
	getchar();
	return 0;
}
