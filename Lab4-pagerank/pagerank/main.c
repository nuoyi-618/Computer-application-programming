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

	while (!feof(url))//统计模式串行数
	{
		c = fgetc(url);
		if (c == '\n')
			n++;
	}
	rewind(url);//将指针重置到第一行
	struct dict* URLs = NULL;
	URLs = (struct dict*)bupt_malloc(sizeof(struct dict) * (n)); //创建一个结构体含有(n+1)个数据
	for (int j = 0; j < n; j++)
	{
		fgets(row, 100, url);    //一次读取一行
		row[strlen(row) - 1] = 0;//去除\n
		sscanf(row, "%s ", &URLs[j].word);
	}

	fclose(url);
	while (!feof(urlcp))
	{
		fgets(row2, 20, urlcp);
		for (space = 0; row2[space] != ' '; ++space);//找到第一个空格
		strncpy(strSourceID, row2, space);
		formerId = atoi(strSourceID);//起点ID
		space += 2;
		for (end = space; row2[end] != '\n'; ++end);//找到结尾
		strncpy(strDestID, row2 + space + 1, end - space - 1);
		latterId = atoi(strDestID);//终点ID

		sourceID = latterId;
		destID = formerId;
		n2++;
		LinkOutNum[sourceID]++;//出度++
		LinkInNum[destID]++;//入度++
		getNumforCoo(sourceID, destID);//行列号存入COO
	}
	printf("%d\n", n2);


	getValueforCoo(LinkOutNum);//获取COO value值

	printf("Now start cal Pagerank\n");

	CalPageRank();

	printf("Now start print TOP URL\n");
	findTop10(URLs);

	free(URLs);
	fclose(url);
	fclose(top10);
	return 0;
}