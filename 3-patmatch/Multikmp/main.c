#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define BUFSIZE 65
#define PATTERNS_N 1500
/*�洢ģʽ��*/
struct dict
{
	char word[65];
	int count;
	int* next;
};
/*ͳ��*/
struct global_stats {
	long long mem;
	unsigned long long cmpnum;
}global_stats;

/*��̬�ڴ�*/
void* bupt_malloc(size_t size) {
	if (size <= 0) {
		return NULL;
	}
	global_stats.mem += size;
	return (malloc(size));
}
void get_next(char* t, int* next) {
	int i = 0, j = -1;
	next[0] = -1;
	while (i < (int)strlen(t)) {
		if (j == -1 || t[i] == t[j]) {
			i++;
			j++;
			next[i] = j;
		}
		else
			j = next[j];
	}
}

int index_KMP(char * s,char * t,int pos,int* next)
{
	int len1 = strlen(s), len2 = strlen(t);
	int n = 0;
	while (pos <= len1 - len2)
	{
		int i = pos, j = 0;
		while (i < len1 && j < len2) {
			if (j == -1 || s[i] == t[j])//�������к����ַ����Ƚ�
			{
				i++;
				j++;
				global_stats.cmpnum++;
			}
			else j = next[j];//ģʽ������
		}

		if (j >= len2) {//ƥ��ɹ�
			n++;
			pos = i - next[j - 1] - 1;
		}
		else//ƥ�䲻�ɹ�
			pos=i-1;
	}
	return n;
}
/*ð������*/
void BubbleSort(struct dict* arr, int size)
{
	int i, j, tmp_count;
	char tmp_word[65];
	for (i = 0; i < size - 1; i++) {
		for (j = 0; j < size - i - 1; j++) {
			if (arr[j].count < arr[j + 1].count) {
				tmp_count = arr[j].count;
				strcpy(tmp_word, arr[j].word);
				arr[j].count = arr[j + 1].count;
				strcpy(arr[j].word, arr[j + 1].word);
				strcpy(arr[j + 1].word, tmp_word);
				arr[j + 1].count = tmp_count;
			}
		}
	}
}
int main() {
	FILE* patterns = fopen("../pattern_bf_kmp.txt", "r");
	FILE* str_f = fopen("../string.txt", "r");
	FILE* result = fopen("./result.txt", "w"); //����һ�������ļ�ֻд
	if (patterns == NULL || str_f == NULL)
	{
		printf("Unable to open this file!\n");
		exit(0);
	}

	char* string = bupt_malloc(880 * 1024 * 1024 * sizeof(char));
	//char* string = bupt_malloc(1024*sizeof(char));
	char c;
	long long i = 0;

	char buf[BUFSIZE];
	int count = 0;
	while (!feof(str_f))
	{
		c = fgetc(str_f);
		string[i++] = c;
	}
	//printf("%lld",i);
	struct dict* dict_words = NULL;
	dict_words = (struct dict*)bupt_malloc(sizeof(struct dict) * (PATTERNS_N + 1)); //����һ���ṹ�庬��(n+1)������
	clock_t start = clock();
	/*����ģʽ��*/
	for (int j = 0; j < PATTERNS_N; j++)
	{
		fgets(buf, BUFSIZE, patterns);    //һ�ζ�ȡһ��
		sscanf(buf, "%s ", &dict_words[j].word);
		dict_words[j].next = (int*)bupt_malloc((strlen(buf) + 1) * sizeof(int));// ����s2��next����ռ�
		get_next(dict_words[j].word, dict_words[j].next);
		dict_words[j].count = index_KMP(string, dict_words[j].word, 0, dict_words[j].next);
		if (j % 100 == 0)
			printf("%s%d\n", dict_words[j].word, dict_words[j].count);
	}
	clock_t end = clock();
	printf("Total searching time(with I/O): %.0lfs\n", (double)((end - start) / CLOCKS_PER_SEC));

	

	BubbleSort(dict_words, PATTERNS_N);

	for (int j = 0; j < PATTERNS_N; j++)
	{
		//printf("%s%d\n", dict_words[j].word, dict_words[j].count);
		fprintf(result, "%s %d\n", dict_words[j].word, dict_words[j].count);
	}
	fprintf(result, "�ַ��Ƚϴ���%lldK  �ڴ�%lldKB\n", global_stats.cmpnum / 1000, global_stats.mem / 1024);
	printf("�ַ��Ƚϴ���%lldK  �ڴ�%lldKB\n", global_stats.cmpnum / 1000, global_stats.mem / 1024);
	fclose(result);
	fclose(patterns);	//�ر��ļ�
	fclose(str_f);	//�ر��ļ�
	
	return 0;
}