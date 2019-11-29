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
	int cnt;//�Ƿ�Ϊ�õ��ʵ����һ����� 
	struct Node* fail;//ʧ��ָ�� 
	struct Node* next[256];//Trie��ÿ�����ĸ����ڵ� 
	struct Node* next_node;
};//���У�������BFS����ʧ��ָ�� 
typedef struct Queue {
	struct Node* front;
	struct Node* rear;
}Queue;

//��ʼ������
Queue* Init_queue() {
	Queue* q = (Queue*)malloc(sizeof(Queue));
	q->front = q->rear = NULL;
	return q;
}

//�ж��Ƿ��
int Qisempty(Queue* q) {
	return q->front == NULL;
}

//���
void Q_insert(Queue* q, struct Node* node) {
	//Q_node *node = (Q_node *)malloc(sizeof(Q_node));
	if (node == NULL)
		return;
	//node->data = data;
	node->next_node = NULL;//β�巨������Ԫ��ָ���
	if (q->rear == NULL)
	{
		q->front = node;
		q->rear = node;
	}
	else {
		q->rear->next_node = node;//��n��Ϊ��ǰ��β���ڵ���һ�ڵ�
		q->rear = node;//β��ָ��ָ��n
	}
}
//����
struct Node* Q_pop(Queue* q) {
	struct Node* node = q->front;
	if (node == NULL)
		return NULL;
	if (q->front == q->rear)//�Ƿ�ֻ��һ��Ԫ��
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

struct Node* root;//ͷ��� 
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
void Init(struct Node* root)//ÿ�����ĳ�ʼ�� 
{
	root->cnt = -1;
	root->fail = NULL;
	for (int i = 0; i < 256; i++)
		root->next[i] = NULL;
	root->next_node = NULL;
}
int* Build_trie(char* keyword)//����Trie�� 
{
	struct Node* p, * q;
	int i, v;
	int len = strlen(keyword);
	for (i = 0, p = root; i < len; i++)
	{
		if (keyword[i] < 0) {
			v = 256+keyword[i];//GB2312�������λΪ����ת��Ϊ��
		}
		else v = keyword[i];
		if (p->next[v] == NULL)
		{
			q = (struct Node*)bupt_malloc(sizeof(struct Node));
			Init(q);
			p->next[v] = q;//������� 
		}
		p = p->next[v];//ָ���ƶ�����һ����� 
	}
	p->cnt=0;//�������һ�����cnt++������һ������ 
	return &p->cnt;
}
void Build_AC_automation(struct Node* trie)
{
	Queue* queue = Init_queue();
	Q_insert(queue, root);
	while (!Qisempty(queue))
	{
		struct Node* p = NULL;
		struct Node* temp = Q_pop(queue); //������ͷ��� 
		for (int i = 0; i < 256; i++)
		{
			if (temp->next[i] != NULL)//�ҵ�ʵ�ʴ��ڵ��ַ���� 
			{ //temp->next[i] Ϊ�ý�㣬tempΪ�丸��� 
				if (temp == root)//���ǵ�һ���е��ַ���㣬��Ѹý���ʧ��ָ��ָ��root 
					temp->next[i]->fail = root;
				else
				{
					//���λ��ݸýڵ�ĸ��ڵ��ʧ��ָ��ֱ��ĳ�ڵ��next[i]��ýڵ���ͬ��
					//��Ѹýڵ��ʧ��ָ��ָ���next[i]�ڵ㣻 
					//�����ݵ� root ��û���ҵ�����ýڵ��ʧ��ָ��ָ�� root
					p = temp->fail;//���ý��ĸ�����ʧ��ָ���p 
					while (p != NULL)
					{
						if (p->next[i] != NULL)
						{
							temp->next[i]->fail = p->next[i];
							break;
						}
						p = p->fail;
					}
					//�øý���ʧ��ָ��Ҳָ��root 
					if (p == NULL)
						temp->next[i]->fail = root;
				}
				Q_insert(queue, temp->next[i]);
			}
		}
	}
	
}
void query(struct Node* root, char* s)
{ //iΪ����ָ�룬pΪģʽ��ָ�� 
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
		//��ʧ��ָ����ݲ��ң��ж�s[i]�Ƿ������Trie���� 
		while (p->next[v] == NULL && p != root)
			p = p->fail;
		p = p->next[v];//�ҵ���pָ��ָ��ý�� 
		global_stats.cmpnum++;
		if (p == NULL)//��ָ�뷵��Ϊ�գ���û���ҵ���֮ƥ����ַ� 
			p = root;
		struct Node* temp = p;//ƥ��ý�������ʧ��ָ����ݣ��ж���������Ƿ�ƥ�� 
		while (temp != root)//ƥ��������� 
		{
			temp->cnt= temp->cnt+1;
			temp = temp->fail;//���� ʧ��ָ�� ����Ѱ����һ�����������Ľ�� 
		}
	}
}
/*ð������*/
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
/*����*/
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
	FILE* result = fopen("./result.txt", "w"); //����һ�������ļ�ֻд
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
	while (!feof(patterns))//ͳ��ģʽ������
	{
		c = fgetc(patterns);
		if (c == '\n')
			patterns_n++;
	}
	rewind(patterns);//��ָ�����õ���һ��
	struct dict* dict_words = NULL;
	dict_words = (struct dict*)bupt_malloc(sizeof(struct dict) * (patterns_n )); //����һ���ṹ�庬��(n+1)������
	
	//patterns_n = 2256690;
	clock_t start = clock();
	/*����ģʽ��*/
	
	for (int j = 0; j < patterns_n;j++)
	{
		fgets(keyword, 100, patterns);    //һ�ζ�ȡһ��
		keyword[strlen(keyword) -1]=0;//ȥ��\n
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

	fprintf(result, "�ַ��Ƚϴ���%lldK  �ڴ�%lldKB\n", global_stats.cmpnum / 1000, global_stats.mem / 1024);
	printf("�ַ��Ƚϴ���%lldK  �ڴ�%lldKB\n", global_stats.cmpnum / 1000, global_stats.mem / 1024);
	fclose(result);
	fclose(patterns);	//�ر��ļ�
	fclose(str_f);	//�ر��ļ�
	return 0;
}
