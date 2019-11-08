#include <stdio.h>
#include <stdlib.h>
#include "BPlusTree.h"
#define BUFSIZE 65
#define LENGTH(a) ((sizeof(a))/(sizeof(a[0])))

int main(int argc, const char* argv[])
{
	
	//��ȡ�ļ�
	FILE* patterns;
	FILE* word;
	int i, j, patterns_n = 0;
	char buf[BUFSIZE], c;
	
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
	//�ʿⱣ�浽�ṹ��
	
	rewind(patterns);//��ָ�����õ���һ��
	BPlusTree T;
	T = Initialize(); //��ʼ��
	//char str[65];
	for (i = 0; i < patterns_n; i++)
	{
		fgets(buf, BUFSIZE, patterns);    //һ�ζ�ȡһ��
		char* p;
		p = (char*)bupt_malloc(sizeof(char) * strlen(buf));
		sscanf(buf, "%s ", p);
		T =  Insert(T, p);
	}

	//Travel(T);//����
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
	for (i = 0; i < global_stats.words_n; i++)
	{
		char str[65];
		fgets(buf, BUFSIZE, word);    //һ�ζ�ȡһ��
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

	fprintf(result, "�ڵ���%d �����ṹռ���ڴ���%d   �ַ��Ƚϴ���%d   words����%d   �ɹ���������%d", global_stats.node_num ,global_stats.mem/1024 , global_stats.cmpnum / 1000, global_stats.words_n, global_stats.right);
	printf("�ڵ���%d �����ṹռ���ڴ���%d   �ַ��Ƚϴ���%d   words����%d   �ɹ���������%d", global_stats.node_num , global_stats.mem/1024 , global_stats.cmpnum / 1000, global_stats.words_n, global_stats.right);
	Destroy(T);//����
	fclose(result);
	fclose(patterns);	//�ر��ļ�
	fclose(word);	//�ر��ļ�
	getchar();
	return 0;

	
}
