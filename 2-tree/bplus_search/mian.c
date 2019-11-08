#include <stdio.h>
#include <stdlib.h>
#include "BPlusTree.h"
#define BUFSIZE 65
#define LENGTH(a) ((sizeof(a))/(sizeof(a[0])))

int main(int argc, const char* argv[])
{
	
	//读取文件
	FILE* patterns;
	FILE* word;
	int i, j, patterns_n = 0;
	char buf[BUFSIZE], c;
	
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
	//词库保存到结构体
	
	rewind(patterns);//将指针重置到第一行
	BPlusTree T;
	T = Initialize(); //初始化
	//char str[65];
	for (i = 0; i < patterns_n; i++)
	{
		fgets(buf, BUFSIZE, patterns);    //一次读取一行
		char* p;
		p = (char*)bupt_malloc(sizeof(char) * strlen(buf));
		sscanf(buf, "%s ", p);
		T =  Insert(T, p);
	}

	//Travel(T);//遍历
	int flag = 0;
	FILE* result;
	result = fopen("./result.txt", "w"); //建立一个文字文件只写
	while (!feof(word))
	{
		c = fgetc(word);
		if (c == '\n')
			global_stats.words_n++;
	}
	rewind(word);
	for (i = 0; i < global_stats.words_n; i++)
	{
		char str[65];
		fgets(buf, BUFSIZE, word);    //一次读取一行
		sscanf(buf, "%s ", &str);
		flag = Search(T, str);
		if (flag == 1) {
			fprintf(result, "%syes\n", str);
			global_stats.right++;
		}
		else {
			fprintf(result, "%sno\n", str);
		}
	}

	fprintf(result, "节点数%d 检索结构占用内存量%d   字符比较次数%d   words总数%d   成功检索总数%d", global_stats.node_num ,global_stats.mem/1024 , global_stats.cmpnum / 1000, global_stats.words_n, global_stats.right);
	printf("节点数%d 检索结构占用内存量%d   字符比较次数%d   words总数%d   成功检索总数%d", global_stats.node_num , global_stats.mem/1024 , global_stats.cmpnum / 1000, global_stats.words_n, global_stats.right);
	Destroy(T);//销毁
	fclose(result);
	fclose(patterns);	//关闭文件
	fclose(word);	//关闭文件
	getchar();
	return 0;

	
}
