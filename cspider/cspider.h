#ifndef CSPIDER_H
#define CSPIDER_H
#include <stdio.h>
#include <time.h>


#define maxq 1000000  // 队列最大长度
#define maxu 500000   // 存储的不重复URL最大数量
#define BUF 1000000   // 源码最大长度
#define BF_SIZE 3145728  // 布隆过滤器位数 1024*1024*3
void* bupt_malloc(size_t size);

typedef unsigned int(*hash_function)(char*, unsigned int len);


unsigned int RSHash(char* str, unsigned int len);
unsigned int JSHash(char* str, unsigned int len);
unsigned int PJWHash(char* str, unsigned int len);
unsigned int ELFHash(char* str, unsigned int len);
unsigned int BKDRHash(char* str, unsigned int len);
unsigned int SDBMHash(char* str, unsigned int len);
unsigned int DJBHash(char* str, unsigned int len);
unsigned int DEKHash(char* str, unsigned int len);
unsigned int BPHash(char* str, unsigned int len);
unsigned int FNVHash(char* str, unsigned int len);
unsigned int APHash(char* str, unsigned int len);

unsigned int MyHash(char* str, unsigned int len);
unsigned int MurmurHash2(const void* key, int len, unsigned int seed);

//爬虫
char source_code[BUF];
FILE *url_list, *url_cp, *url, *debug;
char* URLs[maxu];  // 存储所有状态码为200且不重复的URL
int urls_num;      // 数组URLs中元素的个数
time_t start_time, cur_time;  // 用于计时
int duration;                 // 用于计时

typedef struct
{
	char *urlName;
	int fd;
} Ev_arg;

// 树节点
typedef struct TNode
{
	char data;                   //节点存储的字节数据
	struct TNode *left, *right;  //同级节点TSNode
	struct TNode *next;          //下一级节点
	int ID;                      //编号id
} TNode, *TNodePtr;

// 树结构体
typedef struct
{
	TNodePtr root;         //三叉树根节点
	int uNum;              //插入URL的个数
	int uLen;              //插入URL的长度
	char* searchURL;       //待搜索的URL
	TNodePtr currentNode;  //当前搜索到的节点
} TREE;

TREE *tr_alloc();
int add_url(TREE *tree, char *P, int M, int id);
int search(TREE* tree, int uLen, char* searchURL);
void free_tree(TREE *tree);
void node_free(TNodePtr node);

//布隆过滤器
typedef struct
{
	char* bits;
} BF;

void bf_params(void);
void init_bf(BF *bf, int length);// 初始化
void set_bf_bits(BF *bf, char* p);//添加字符串
int check_bf_bits(BF *bf, char* p);//搜索
void free_bf(BF *bf);//释放


// 队列节点
typedef struct que{
	char* url;
	struct que* next;
}QueueNode;

typedef struct {
	int size;
	QueueNode* ql;
	QueueNode* tail;
}Queue;

Queue* create_queue();
QueueNode* create_queue_node(char* url);
void queue_add(Queue* q, char* url);
char* queue_pop(Queue* q);
int queue_is_empty(Queue* q);
int queue_length(Queue* q);
void queue_destroy(Queue* q);

// 从该源码中提取的符合条件的URL全部入栈q
int DFA(char* souceCode, Queue *q, BF *bf, int id);
//并行爬取网页
int handleURLs(Queue *queue_url, BF *bf, char* serverip, int port);
int buildConnect(int *sockfd, char* serverip, int port);
int setnoblocking(int sockfd);
int sendRequest(char* url, int sockfd, char* serverip);
// 递归创建目录
void mkdirs(char *muldir);

#endif