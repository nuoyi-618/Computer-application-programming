#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"pagerank.h"

int LinkInNum[MAX] = { 0 };
int* LinkOutNum[MAX] = { 0 };
int main(int argc, char* argv[])
{

	FILE* top10 = fopen("result.txt", "w");
	FILE* url = fopen("./urllist.txt", "r");
	FILE* urlcp = fopen("./urlcp.txt", "r");

	if (url == NULL || urlcp == NULL)
	{
		printf("Unable to open this file!\n");
		exit(0);
	}

	char row[300], row2[300];
	char strSourceID[20], strDestID[20];
	int space = 0, end = 0, sourceID = 0, destID = 0;
	int formerId = 0, latterId = 0;
	int flag = 0, n = 0, n2 = 0;
	char c;

	while (!feof(url))//ͳ��ģʽ������
	{
		c = fgetc(url);
		if (c == '\n')
			n++;
	}
	rewind(url);//��ָ�����õ���һ��
	struct dict* URLs = NULL;
	URLs = (struct dict*)bupt_malloc(sizeof(struct dict) * (n)); //����һ���ṹ�庬��(n+1)������
	for (int j = 0; j < n; j++)
	{
		fgets(row, 100, url);    //һ�ζ�ȡһ��
		row[strlen(row) - 1] = 0;//ȥ��\n
		sscanf(row, "%s ", &URLs[j].word);
	}

	fclose(url);
	while (!feof(urlcp))
	{
		fgets(row2, 20, urlcp);
		for (space = 0; row2[space] != ' '; ++space);//�ҵ���һ���ո�
		strncpy(strSourceID, row2, space);
		formerId = atoi(strSourceID);//���ID
		space += 2;
		for (end = space; row2[end] != '\n'; ++end);//�ҵ���β
		strncpy(strDestID, row2 + space + 1, end - space - 1);
		latterId = atoi(strDestID);//�յ�ID

		sourceID = latterId;
		destID = formerId;
		n2++;
		LinkOutNum[sourceID]++;//����++
		LinkInNum[destID]++;//���++
		getNumforCoo(sourceID, destID);//���кŴ���COO
	}
	printf("%d\n", n2);


	getValueforCoo(LinkOutNum);//��ȡCOO valueֵ

	printf("Now start cal Pagerank\n");

	CalPageRank();

	printf("Now start print TOP URL\n");
	findTop10(URLs);

	free(URLs);
	fclose(url);
	fclose(top10);
	return 0;
}