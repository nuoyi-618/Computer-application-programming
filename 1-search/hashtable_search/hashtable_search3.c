#include<stdio.h>
#include<stdlib.h>
#include < assert.h >
#define BUFSIZE 100
long mem = 0;
long cmpnum = 0;
struct keyNum
{
	char key[100];//�ؼ���
	struct keyNum* next;
};
/*****��̬�ڴ�******/
void* bupt_malloc(size_t size) {
	if (size <= 0)
	{
		return NULL;
	}

	mem += size;
	return malloc(size);
}
/*****���뵽��ϣ��******/
struct keyNum* insertHash(struct keyNum* head, char s[100])
{
	struct keyNum* p0, * p1,*p2, * temp;
	temp = (struct keyNum*)bupt_malloc(sizeof(struct keyNum));
	strcpy(temp->key, s);
	//temp->key = s;
	p1 = head;
	p0 = temp;//Ҫ����Ľڵ㣨ֵΪm);
	if (head == NULL)//1��ԭ��������Ϊ�գ����뵽head��
	{
		head = p0;
		p0->next = NULL;
	}
	else//ԭ��������Ϊ��
	{
		while ((p0->key > p1->key) && (p1->next != NULL))//�ƶ����ʵ�λ��
		{
			p2 = p1;
			p1 = p1->next;
		}
		if (p0->key <= p1->key)
		{
			if (head == p1)head = p0;//2�����뵽��һ���ڵ�֮ǰ
			else p2->next = p0;//3,���뵽p2ָ��Ľڵ�֮��
			p0->next = p1;

		}
		else//4,���뵽��β��
		{
			p1->next = p0;
			p0->next = NULL;

		}
	}

	return(head);
}
/*****��������head���Ƿ����m******/
int searchHash(struct keyNum* head, char m[100])
{

	struct keyNum* p;
	p = head;
	while (p != NULL) {
		if (bupt_strcmp(p->key, m) == 0) {
			return 1;
		}
		else p = p->next;
	}
	return 0;
}



/*****�ַ��Ƚ�******/
int byte_cmp(char a, char b) {
	cmpnum++;
	return a - b;
}
/*****�ַ����Ա�******/
int bupt_strcmp(const char* str1, const char* str2)
{
	while (byte_cmp(*str1, *str2) == 0)
	{
		assert((str1 != NULL) && (str2 != NULL));
		if (*str1 == '\0')
			return 0;
		str1++;
		str2++;
	}
	return *str1 - *str2;
}

/*****��ϣ����******/
unsigned int PJWHash(const char* str, unsigned int length)
{
	const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
	const unsigned int ThreeQuarters = (unsigned int)((BitsInUnsignedInt * 3) / 4);
	const unsigned int OneEighth = (unsigned int)(BitsInUnsignedInt / 8);
	const unsigned int HighBits =
		(unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
	unsigned int hash = 0;
	unsigned int test = 0;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = (hash << OneEighth) + (*str);

		if ((test = hash & HighBits) != 0)
		{
			hash = ((hash ^ (test >> ThreeQuarters))& (~HighBits));
		}
	}

	return hash;
}
void free_vector(struct keyNum* pf) {
	if (pf == NULL) return;
	else {
		pf = pf->next;
		free_vector(pf);
		free(pf);
	}
}
void main()
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
	while (!feof(word))
	{
		c = fgetc(word);
		if (c == '\n')
			words_n++;
	}
//	printf("words�ı���=%d\n", words_n);//���ı���
	/*******�ʿⱣ�浽�ṹ��******/
	rewind(patterns);//��ָ�����õ���һ��
	struct keyNum** hash = (struct keyNum**)bupt_malloc(sizeof(struct keyNum*) * (patterns_n+1)); //����һ���ṹ�庬��(n+1)������
	
	for (i = 1; i <=patterns_n; i++) {//��ʼ��
		hash[i] = NULL;
	}
	int len, m;

	
	for (i = 1; i <= patterns_n; i++)
	{
		
		fgets(buf, BUFSIZE, patterns);    //һ�ζ�ȡһ��
		sscanf(buf, "%s ", &str);
		len = strlen(buf);
		m = PJWHash(buf,len) % (patterns_n);//����õ��ؼ��ֵĹ�ϣֵ
		hash[m] = insertHash(hash[m], str);//���ؼ���k���뵽��ϣֵΪm��������
	}
	rewind(word);
	int flag=0,right=0;
	FILE* result;
	result = fopen("./result.txt", "w"); //����һ�������ļ�ֻд
	for (i = 0; i < words_n; i++)
	{
		
		fgets(buf, BUFSIZE, word);    //һ�ζ�ȡһ��
		sscanf(buf, "%s ", &str);
		len = strlen(buf);
		m = PJWHash(buf, len) % (patterns_n);//����õ��ؼ��ֵĹ�ϣֵ
		
		flag = searchHash(hash[m], str);
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

	for (int i = 1; i < patterns_n+1; i++) {
		struct keyNum* p = hash[i];
		free_vector(hash[i]);
	}
	
	free(hash);
	fclose(result);
	fclose(patterns);	//�ر��ļ�
	fclose(word);	//�ر��ļ�
	getchar();
	return 0;
}


