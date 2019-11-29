#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct dict
{
	char word[100];
	int* count;
	int* next;
};

struct Node
{
	int cnt;//是否为该单词的最后一个结点 
	struct Node* fail;//失败指针 
	struct Node* next[256];//Trie中每个结点的各个节点 
	struct Node* next_node;
};//队列，方便用BFS构造失败指针 
typedef struct Queue {
	struct Node* front;
	struct Node* rear;
}Queue;

//初始化队列
Queue* Init_queue() {
	Queue* q = (Queue*)malloc(sizeof(Queue));
	q->front = q->rear = NULL;
	return q;
}

//判断是否空
int Qisempty(Queue* q) {
	return q->front == NULL;
}

//入队
void Q_insert(Queue* q, struct Node* node) {
	//Q_node *node = (Q_node *)malloc(sizeof(Q_node));
	if (node == NULL)
		return;
	//node->data = data;
	node->next_node = NULL;//尾插法，插入元素指向空
	if (q->rear == NULL)
	{
		q->front = node;
		q->rear = node;
	}
	else {
		q->rear->next_node = node;//让n成为当前的尾部节点下一节点
		q->rear = node;//尾部指针指向n
	}
}
//出队
struct Node* Q_pop(Queue* q) {
	struct Node* node = q->front;
	if (node == NULL)
		return NULL;
	if (q->front == q->rear)//是否只有一个元素
	{
		q->front = NULL;
		q->rear = NULL;
		return node;
	}
	else {
		q->front = q->front->next_node;
		return node;
	}
}

struct Node* root;//头结点 
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
void Init(struct Node* root)//每个结点的初始化 
{
	root->cnt = -1;
	root->fail = NULL;
	for (int i = 0; i < 256; i++)
		root->next[i] = NULL;
	root->next_node = NULL;
}
int* Build_trie(char* keyword)//构建Trie树 
{
	struct Node* p, * q;
	int i, v;
	int len = strlen(keyword);
	for (i = 0, p = root; i < len; i++)
	{
		if (keyword[i] < 0) {
			v = 256+keyword[i];//GB2312中文最高位为负，转换为正
		}
		else v = keyword[i];
		if (p->next[v] == NULL)
		{
			q = (struct Node*)bupt_malloc(sizeof(struct Node));
			Init(q);
			p->next[v] = q;//结点链接 
		}
		p = p->next[v];//指针移动到下一个结点 
	}
	p->cnt=0;//单词最后一个结点cnt++，代表一个单词 
	return &p->cnt;
}
void Build_AC_automation(struct Node* trie)
{
	Queue* queue = Init_queue();
	Q_insert(queue, root);
	while (!Qisempty(queue))
	{
		struct Node* p = NULL;
		struct Node* temp = Q_pop(queue); //弹出队头结点 
		for (int i = 0; i < 256; i++)
		{
			if (temp->next[i] != NULL)//找到实际存在的字符结点 
			{ //temp->next[i] 为该结点，temp为其父结点 
				if (temp == root)//若是第一层中的字符结点，则把该结点的失败指针指向root 
					temp->next[i]->fail = root;
				else
				{
					//依次回溯该节点的父节点的失败指针直到某节点的next[i]与该节点相同，
					//则把该节点的失败指针指向该next[i]节点； 
					//若回溯到 root 都没有找到，则该节点的失败指针指向 root
					p = temp->fail;//将该结点的父结点的失败指针给p 
					while (p != NULL)
					{
						if (p->next[i] != NULL)
						{
							temp->next[i]->fail = p->next[i];
							break;
						}
						p = p->fail;
					}
					//让该结点的失败指针也指向root 
					if (p == NULL)
						temp->next[i]->fail = root;
				}
				Q_insert(queue, temp->next[i]);
			}
		}
	}
	
}
void query(struct Node* root, char* s)
{ //i为主串指针，p为模式串指针 
	long long i;
	int v;
	struct Node* p = root;
	long long len = strlen(s);
	for (i = 0; i < len; i++)
	{
		if (s[i] < 0) {
			v = 256+s[i];
		}
		else v = s[i];
		//由失败指针回溯查找，判断s[i]是否存在于Trie树中 
		while (p->next[v] == NULL && p != root)
			p = p->fail;
		p = p->next[v];//找到后p指针指向该结点 
		global_stats.cmpnum++;
		if (p == NULL)//若指针返回为空，则没有找到与之匹配的字符 
			p = root;
		struct Node* temp = p;//匹配该结点后，沿其失败指针回溯，判断其它结点是否匹配 
		while (temp != root)//匹配结束控制 
		{
			temp->cnt= temp->cnt+1;
			temp = temp->fail;//回溯 失败指针 继续寻找下一个满足条件的结点 
		}
	}
}
/*冒泡排序*/
void BubbleSort(struct dict* arr, int size)
{
	int i, j;
	int *tmp_count;
	char tmp_word[100];
	for (i = 0; i < size - 1; i++) {
		for (j = 0; j < size - i - 1; j++) {
			if (*arr[j].count < *arr[j + 1].count) {
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
/*快排*/
void quick_sort(struct dict* arr, int l, int r)
{
	int i = l, j = r;
	char tmp_word[100];
	strcpy(tmp_word,arr[l].word);
	int* tmp_count = arr[l].count;
	while (i < j)
	{
		while (i < j && (*tmp_count) > (*arr[j].count))
		{
			j--;
		}
		if (i < j)
		{
			arr[i++] = arr[j];
		}
		while (i < j && (*tmp_count) < (*arr[i].count))
		{
			i++;
		}
		if (i < j)
		{
			arr[j--] = arr[i];
		}
	}
	strcpy(arr[i].word, tmp_word); 
	arr[i].count = tmp_count;
	if (l < i)
	{
		quick_sort(arr, l, i - 1);
	}
	if (r > i)
	{
		quick_sort(arr, i + 1, r);
	}

}



int main()
{
	FILE* patterns = fopen("../pattern.txt", "r");
	FILE* str_f = fopen("../string.txt", "r");
	FILE* result = fopen("./result.txt", "w"); //建立一个文字文件只写
	/*
	int n = 0;
	char s[1000], t[1000];
	while (!feof(patterns))
	{
		fgets(s, 100, patterns);
		if (strlen(s) > n) {
			strcpy(t, s);
			n = strlen(s);
		}
	}
	printf("%d\n", n);*/
	


	if (patterns == NULL || str_f == NULL)
	{
		printf("Unable to open this file!\n");
		exit(0);
	}
	char* string = bupt_malloc(880 * 1024 * 1024 * sizeof(char));
	char c;
	long long i = 0;
	int count = 0;
	while (!feof(str_f))
	{
		c = fgetc(str_f);
		string[i++] = c;
	}
	int patterns_n=1;
	char keyword[100];
	root = (struct Node*)bupt_malloc(sizeof(struct Node));
	Init(root);
	while (!feof(patterns))//统计模式串行数
	{
		c = fgetc(patterns);
		if (c == '\n')
			patterns_n++;
	}
	rewind(patterns);//将指针重置到第一行
	struct dict* dict_words = NULL;
	dict_words = (struct dict*)bupt_malloc(sizeof(struct dict) * (patterns_n )); //创建一个结构体含有(n+1)个数据
	
	//patterns_n = 2256690;
	clock_t start = clock();
	/*查找模式串*/
	
	for (int j = 0; j < patterns_n;j++)
	{
		fgets(keyword, 100, patterns);    //一次读取一行
		keyword[strlen(keyword) -1]=0;//去除\n
		sscanf(keyword, "%s ", &dict_words[j].word);
		dict_words[j].count=Build_trie(keyword);
	}
	Build_AC_automation(root);
	query(root,string);
	clock_t end = clock();
	printf("Total searching time(with I/O): %.0lfs\n", (double)((end - start) / CLOCKS_PER_SEC));

//	BubbleSort(dict_words, patterns_n);
	quick_sort(dict_words,0, patterns_n-1);

	for (int j = 0; j < patterns_n; j++)
	{
		//printf("%s%d\n", dict_words[j].word, dict_words[j].count);
		fprintf(result, "%s %d\n", dict_words[j].word, *dict_words[j].count);
	}

	fprintf(result, "字符比较次数%lldK  内存%lldKB\n", global_stats.cmpnum / 1000, global_stats.mem / 1024);
	printf("字符比较次数%lldK  内存%lldKB\n", global_stats.cmpnum / 1000, global_stats.mem / 1024);
	fclose(result);
	fclose(patterns);	//关闭文件
	fclose(str_f);	//关闭文件
	return 0;
}
