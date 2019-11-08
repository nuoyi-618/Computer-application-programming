#include <stdio.h>
#include "radix4tree.h"
#define BUFSIZE 65
int main() {
	//初始化一棵radix_tree
	struct rt_node* T = Init_rdx_tree();
	//读取patterns
	FILE* patterns;
	FILE* word;
	patterns = fopen("./patterns-127w.txt", "r");
	word = fopen("./words-98w.txt", "r");
	char buf[BUFSIZE], c;
	int patterns_n = 0;
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
	//插入patterns到radix_tree
	rewind(patterns);
	for (int i = 0; i < patterns_n; i++)
	{
		fgets(buf, BUFSIZE, patterns);    //一次读取一行
		char* p;
		p = (char*)malloc(sizeof(char) * strlen(buf));
		sscanf(buf, "%s ", p);
		struct Bits* bits = init_bits(p);
		Insert(T, bits);
	}
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
	for (int i = 0; i < global_stats.words_n; i++)
	{
		fgets(buf, BUFSIZE, word);    //一次读取一行
		char str[65];
		sscanf(buf, "%s ", &str);
		struct Bits* bits = init_bits(str);
		int r = Search(T, bits);
		if (r == 1) {
			global_stats.right++;
		//	printf("%s yes\n", p);
			fprintf(result, "%s %s\n", str, "yes");
		}
		else {
			//printf("%s no\n", str);
			fprintf(result, "%s %s\n", str, "no");
		}
	}

	fprintf(result, "字节总数%lld 内存%lldKB 字符比较次数%lldK  words总个数%d 成功检索的word总个数%d\n",  global_stats.node_num,global_stats.mem / 1024, global_stats.cmp_num / 1000, global_stats.words_n, global_stats.right);
	printf("字节总数%lld 内存%lldKB 字符比较次数%lldK  words总个数%d 成功检索的word总个数%d\n", global_stats.node_num, global_stats.mem / 1024, global_stats.cmp_num / 1000, global_stats.words_n, global_stats.right);
	fclose(result);
	fclose(patterns);	//关闭文件
	fclose(word);	//关闭文件
	getchar();
	return 0;
}