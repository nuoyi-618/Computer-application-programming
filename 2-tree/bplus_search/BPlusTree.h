#pragma once

#ifndef BPlusTree_h
#define BPlusTree_h

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define M (4)
#define LIMIT_M_2 (M % 2 ? (M + 1)/2 : M/2)
struct{
	long node_num;//节点数
	long cmpnum;//字符比较次数
	long mem;//内存大小
	long words_n;//总词数
	long right;//正确的个数
}global_stats;

typedef struct BPlusNode* BPlusTree, * Position;
typedef char* KeyType;
struct BPlusNode {
	int KeyNum;
	KeyType Key[M + 1];
	BPlusTree Children[M + 1];
	BPlusTree Next;
}node;

/* 初始化 */
extern BPlusTree Initialize();
/* 插入 */
extern BPlusTree Insert(BPlusTree T, KeyType Key);
/* 销毁 */
extern BPlusTree Destroy(BPlusTree T);
/* 遍历节点 */
extern void Travel(BPlusTree T);
/* 遍历树叶节点的数据 */
extern void TravelData(BPlusTree T);
/*查找节点*/
int Search(BPlusTree T, KeyType Key);
#endif BPlusTree_h 