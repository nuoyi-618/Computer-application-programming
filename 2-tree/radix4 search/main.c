#include <stdio.h>
#include "radix4tree.h"
#define BUFSIZE 65
int main() {
	//��ʼ��һ��radix_tree
	struct rt_node* T = Init_rdx_tree();
	//��ȡpatterns
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
	//�����ļ������ݵ�����
	while (!feof(patterns))
	{
		c = fgetc(patterns);
		if (c == '\n')
			patterns_n++;
	}
	//����patterns��radix_tree
	rewind(patterns);
	for (int i = 0; i < patterns_n; i++)
	{
		fgets(buf, BUFSIZE, patterns);    //һ�ζ�ȡһ��
		char* p;
		p = (char*)malloc(sizeof(char) * strlen(buf));
		sscanf(buf, "%s ", p);
		struct Bits* bits = init_bits(p);
		Insert(T, bits);
	}
	int flag = 0;
	FILE* result;
	result = fopen("./result.txt", "w"); //����һ�������ļ�ֻд
	while (!feof(word))
	{
		c = fgetc(word);
		if (c == '\n')
			global_stats.words_n++;
	}
	rewind(word);
	for (int i = 0; i < global_stats.words_n; i++)
	{
		fgets(buf, BUFSIZE, word);    //һ�ζ�ȡһ��
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

	fprintf(result, "�ֽ�����%lld �ڴ�%lldKB �ַ��Ƚϴ���%lldK  words�ܸ���%d �ɹ�������word�ܸ���%d\n",  global_stats.node_num,global_stats.mem / 1024, global_stats.cmp_num / 1000, global_stats.words_n, global_stats.right);
	printf("�ֽ�����%lld �ڴ�%lldKB �ַ��Ƚϴ���%lldK  words�ܸ���%d �ɹ�������word�ܸ���%d\n", global_stats.node_num, global_stats.mem / 1024, global_stats.cmp_num / 1000, global_stats.words_n, global_stats.right);
	fclose(result);
	fclose(patterns);	//�ر��ļ�
	fclose(word);	//�ر��ļ�
	getchar();
	return 0;
}