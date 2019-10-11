#include <stdio.h>
#include<stdlib.h>
#include"BloomFilter.h"
#include"BitMap.h"
#define BUFSIZE 100

int main() {
	/*******��ȡ�ļ�*******/

	FILE* patterns;
	FILE* word;
	int i, j, patterns_n = 0, words_n = 0;
	char buf[BUFSIZE], str[BUFSIZE], c;
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
//printf("patterns�ı���=%d\n", patterns_n);//���ı���
	while (!feof(word))
	{
		c = fgetc(word);
		if (c == '\n')
			words_n++;
	}
	//printf("words�ı���=%d\n", words_n);//���ı���
		/*******�ʿⱣ�浽λͼ******/
	rewind(patterns);//��ָ�����õ���һ��
	int len,m;
	BloomFilter bf;
	BloomFilterInit(&bf);//��ʼ��
	for (i = 1; i <= patterns_n; i++)
	{

		fgets(buf, BUFSIZE, patterns);    //һ�ζ�ȡһ��
		sscanf(buf, "%s ", &str);
		len = strlen(buf);
		BloomFilterInsert(&bf, str, len);//���ؼ��ֲ��뵽��¡��������
	}
	rewind(word);
	int flag = 0, right = 0;
	FILE* result;
	result = fopen("./result.txt", "w"); //����һ�������ļ�ֻд
	for (i = 0; i < words_n; i++)
	{

		fgets(buf, BUFSIZE, word);    //һ�ζ�ȡһ��
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
	fprintf(result, "�����ṹռ���ڴ���%d   �ַ��Ƚϴ���%d   words����%d   �ɹ���������%d", mem / 1024, cmpnum, words_n, right);
	printf("�����ṹռ���ڴ���%d   �ַ��Ƚϴ���%d   words����%d   �ɹ���������%d", mem / 1024, cmpnum, words_n, right);
	BloomFilterDestory(&bf);
	getchar();
	return 0;
}
