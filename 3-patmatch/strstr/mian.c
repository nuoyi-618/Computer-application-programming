#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define BUFSIZE 65
#define PATTERNS_N 1500
/*存储模式串*/
struct dict
{
	char word[65];
	int count;
};
/*统计*/
struct global_stats {
	long long mem;
	unsigned long long cmpnum;
}global_stats;

/*动态内存*/
void* bupt_malloc(size_t size) {
	if (size <= 0) {
		return NULL;
	}
	global_stats.mem += size;
	return (malloc(size));
}

/*暴力循环查找模式串*/
int strContrast(const char* str1, const char* str2) {
	int n = 0;
	int pos = 0;
	while (str1[pos] != '\0') {
		int i = pos;
		int j = 0;
		while (str2[j] != '\0') {
			if ((str1[i] - str2[j]) == 0) {
				global_stats.cmpnum++;
				i++;
				j++;
			}
			else break;
			if (str2[j] == '\0') {
				n++;
				break;
			}
		}
		pos++;
	}
	return n;
}
/*冒泡排序*/
void BubbleSort(struct dict* arr, int size)
{
	int i, j, tmp_count;
	char tmp_word[65];
	for (i = 0; i < size - 1; i++) {
		for (j = 0; j < size - i - 1; j++) {
			if (arr[j].count < arr[j + 1].count) {
				tmp_count = arr[j].count;
				strcpy(tmp_word , arr[j].word);
				arr[j].count = arr[j + 1].count;
				strcpy(arr[j].word, arr[j+1].word);
				strcpy(arr[j + 1].word,tmp_word);
				arr[j + 1].count = tmp_count;
			}
		}
	}
}
int main() {
	FILE* patterns = fopen("../pattern_bf_kmp.txt", "r");
	FILE* str_f = fopen("../string.txt", "r");
	FILE* result = fopen("./result.txt", "w"); //建立一个文字文件只写
	if (patterns == NULL || str_f == NULL)
	{
		printf("Unable to open this file!\n");
		exit(0);
	}

	char* string = bupt_malloc(880 * 1024 * 1024 * sizeof(char));
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
	dict_words = (struct dict*)bupt_malloc(sizeof(struct dict) * (PATTERNS_N + 1)); //创建一个结构体含有(n+1)个数据
	/*查找模式串*/
	clock_t start = clock();
	for (int j = 0; j < PATTERNS_N; j++)
	{
		fgets(buf, BUFSIZE, patterns);    //一次读取一行
		sscanf(buf, "%s ", &dict_words[j].word);
		dict_words[j].count = strContrast(string, dict_words[j].word);
		if(j%100==0)
			printf("%s%d\n", dict_words[j].word, dict_words[j].count);
	}
	clock_t end = clock();
	/*排序*/
	BubbleSort(dict_words, PATTERNS_N);
	/*写入文本*/
	for (int j = 0; j < PATTERNS_N; j++)
	{
		//printf("%s%d\n", dict_words[j].word, dict_words[j].count);
		fprintf(result, "%s %d\n", dict_words[j].word, dict_words[j].count);
	}
	fprintf(result, "字符比较次数%lldK  内存%lldKB\n", global_stats.cmpnum/1000, global_stats.mem / 1024);
	printf("字符比较次数%lldK  内存%lldKB\n", global_stats.cmpnum / 1000, global_stats.mem / 1024);
	fclose(result);
	fclose(patterns);	//关闭文件
	fclose(str_f);	//关闭文件
	printf("Total searching time: %.0lfs\n", (double)((end - start) / CLOCKS_PER_SEC));

	return 0;
}