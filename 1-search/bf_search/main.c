#include <stdio.h>
#include<stdlib.h>
#include"BloomFilter.h"
#include"BitMap.h"
#define BUFSIZE 100

int main() {
	/*******读取文件*******/

	FILE* patterns;
	FILE* word;
	int i, j, patterns_n = 0, words_n = 0;
	char buf[BUFSIZE], str[BUFSIZE], c;
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
//printf("patterns文本长=%d\n", patterns_n);//总文本长
	while (!feof(word))
	{
		c = fgetc(word);
		if (c == '\n')
			words_n++;
	}
	//printf("words文本长=%d\n", words_n);//总文本长
		/*******词库保存到位图******/
	rewind(patterns);//将指针重置到第一行
	int len,m;
	BloomFilter bf;
	BloomFilterInit(&bf);//初始化
	for (i = 1; i <= patterns_n; i++)
	{

		fgets(buf, BUFSIZE, patterns);    //一次读取一行
		sscanf(buf, "%s ", &str);
		len = strlen(buf);
		BloomFilterInsert(&bf, str, len);//将关键字插入到布隆过滤器中
	}
	rewind(word);
	int flag = 0, right = 0;
	FILE* result;
	result = fopen("./result.txt", "w"); //建立一个文字文件只写
	for (i = 0; i < words_n; i++)
	{

		fgets(buf, BUFSIZE, word);    //一次读取一行
		sscanf(buf, "%s ", &str);
		len = strlen(buf);
		flag = BloomFilterExit(&bf, str, len);
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
	BloomFilterDestory(&bf);
	getchar();
	return 0;
}
