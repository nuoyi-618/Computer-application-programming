#include <stdio.h>
#include <stdlib.h>
#include "avltree.h"
#define BUFSIZE 65
#define LENGTH(a) ((sizeof(a))/(sizeof(a[0])))
int main()
{

	/*******��ȡ�ļ�*******/
	FILE* patterns;
	FILE* word;
	int i, j, patterns_n = 0, words_n = 0;
	char buf[BUFSIZE], c;
	char str[100];
	//���ļ�
	patterns = fopen("./patterns-127w.txt", "r");
	word = fopen("./words-98w.txt", "r");

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
	//	printf("patterns�ı���=%d\n", patterns_n);//���ı���
	/*******�ʿⱣ�浽�ṹ��******/
	rewind(patterns);//��ָ�����õ���һ��

	avltree tree = NULL;
	for (i = 0; i < patterns_n; i++)
	{
		fgets(buf, BUFSIZE, patterns);    //һ�ζ�ȡһ��
		sscanf(buf, "%s ", &str);
		tree = avltree_insertNode(tree, str);
	}
	int flag = 0, right = 0;
	FILE* result;
	result = fopen("./result.txt", "w"); //����һ�������ļ�ֻд
	while (!feof(word))
	{
		c = fgetc(word);
		if (c == '\n')
			global_stats.words_n++;
	}
	rewind(word);
	for (i = 0; i < global_stats.words_n; i++)
	{
		fgets(buf, BUFSIZE, word);    //һ�ζ�ȡһ��
		sscanf(buf, "%s ", &str);
		flag = search_node(tree, str);
		if (flag == 1) {
			fprintf(result, "%syes\n", str);
			global_stats.right++;
		}
		else {
			fprintf(result, "%sno\n", str);
		}
	}

	fprintf(result, "�ڵ���%d �����ṹռ���ڴ���%d   �ַ��Ƚϴ���%d   words����%d   �ɹ���������%d", global_stats.node_num, global_stats.mem / 1024, global_stats.cmpnum/1000, global_stats.words_n, global_stats.right);
	printf("�ڵ���%d �����ṹռ���ڴ���%d   �ַ��Ƚϴ���%d   words����%d   �ɹ���������%d", global_stats.node_num, global_stats.mem / 1024, global_stats.cmpnum / 1000, global_stats.words_n, global_stats.right);

	fclose(result);
	fclose(patterns);	//�ر��ļ�
	fclose(word);	//�ر��ļ�
	getchar();
	return 0;
}
