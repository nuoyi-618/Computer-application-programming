#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include"pagerank.h"



int coo_row[COO_LENGTH] = { 0 };
int coo_column[COO_LENGTH] = { 0 };
float coo_values[COO_LENGTH] = { 0 };
float pagerank[MAX] = { 1 };

int LinkOutNum[MAX] = { 0 };
int LinkInNum[MAX] = { 0 };

int COONUM = 0;
int URLNUM = 0; 
int RelationNUM = 0;

int top10ID[TOPNUM] = { 0 };

long long mem=0;

/*��̬�ڴ�*/
void* bupt_malloc(size_t size) {
	if (size <= 0) {
		return NULL;
	}
	mem += size;
	return (malloc(size));
}
int main(int argc, char* argv[])
{

	FILE* top10=fopen("./top10.txt", "w");
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
	int flag = 0, n = 0,n2=0;
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
		URLNUM++;
	}

	//printf("%d\n", URLNUM);
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
	//printf("%d\n",n2);

	
	getValueforCoo();//��ȡCOO valueֵ
	CalPageRank();//����PageRank
	findTop10(URLs);//Ѱ��ǰ10
	printf("The top 10 URL is:\n");
	for (int i = 1; i <= TOPNUM; i++)
	{
		fprintf(top10, "%s %.16lf\n", URLs[top[i].ID].word,top[i].Pagerank);
		printf(" %d  URL:%s  %.16lf\n", i, URLs[top[i].ID].word, top[i].Pagerank);
	}
	fprintf(top10,"%d\n",mem);
	printf("%d\n", mem/1024);
	free(URLs);
	fclose(url);
	fclose(urlcp);
	fclose(top10);
	return 0;
}

void getNumforCoo(int source, int dest)
{
	static int coo_num = 0;
	if (coo_num == COO_LENGTH)
	{
		printf("ERROR : the coo has been full \n");
	}
	else
	{
		coo_column[coo_num] = source;//�д�Դ��
		coo_row[coo_num] = dest;//�д��յ�
		coo_num++;
		RelationNUM++;
	}

}

void getValueforCoo()
{
	for (int i = 0; i < RelationNUM; i++)
	{
		int sourceID = coo_column[i];   //��ȡԴ��
		int linkNum = LinkOutNum[sourceID];//���Դ�����
		float value = (float)0.85 * 1.0 / linkNum;  //����Ȩ�أ�����ϵ��0.15��other value = 0.15/MAx
		coo_values[i] = value;
	}
}
void CalPageRank()
{

	if (ELL_COLUMN != 0)
	{
		printf("ERROR: When ELL_COLUMN !=0, Can not use this function!");
		return;
	}

	float* lastrank = bupt_malloc(COO_LENGTH * sizeof(float));
	for (int i = 0; i < COO_LENGTH; i++)
	{
		lastrank[i] = 0;
	}

	float limitation = 0.0001;
	float distance = 1;

	float comf = 0.15 / URLNUM;
	float comp = 0;

	//��������ֵʱ
	while (distance > limitation* limitation)
	{
		comp = 0;
		for (int i = 0; i < URLNUM; i++)
		{
			lastrank[i] = pagerank[i];
			pagerank[i] = 0;
			comp = comp + lastrank[i];
		}
		for (int i = 0; i < URLNUM; i++)
		{
			pagerank[i] = comp * comf;
		}
		for (int i = 0; i < COO_LENGTH; i++)
		{
			pagerank[coo_row[i]] = coo_values[i] * lastrank[coo_column[i]] + pagerank[coo_row[i]];
			pagerank[coo_row[i]] = pagerank[coo_row[i]] - comf * lastrank[coo_column[i]];
		}
		distance = 0;
		for (int i = 0; i < URLNUM; i++)
		{
			distance = (lastrank[i] - pagerank[i]) * (lastrank[i] - pagerank[i]) + distance;
		}
	}
	if (distance < limitation * limitation)//��һ��
	{
		float min = pagerank[0], max = 0;
		for (int i = 0; i < URLNUM; i++)
		{
			if (pagerank[i]<min)
			{
				min = pagerank[i];
			}
			else if (pagerank[i] > max) {
				max = pagerank[i];
			}
		}
		for (int i = 0; i < URLNUM; i++)
		{
			pagerank[i] = (pagerank[i] - min) / (max - min);
		}
	}
}


void findTop10(struct dict* URLs)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < COO_LENGTH; i++)
	{
		for (j = 0; j < TOPNUM+1; j++)
		{
			if (pagerank[i] > top[j].Pagerank)
			{
				for (int q = TOPNUM-1; q > j; q--)
				{
					top[q].Pagerank = top[q - 1].Pagerank;
					top[q].ID = top[q - 1].ID;
				}
				top[j].Pagerank = pagerank[i];
				top[j].ID = i;  
				break;
			}
		}
	}
	return top;
}
