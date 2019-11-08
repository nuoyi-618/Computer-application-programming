#pragma once
#ifndef RADIX4_SEARCH_RADIX4TREE_H
#define RADIX4_SEARCH_RADIX4TREE_H
#include "bit.h"
/*统计计数*/
struct global_stats {
	long long mem;
	unsigned long long cmp_num;
	unsigned int words_n;
	unsigned int right;
	long long node_num;
}global_stats;
/*定义radix_tree 节点*/
typedef struct rt_node {
	struct rt_node* parent;
	struct rt_node* child[RADIX_NODE_SIZE];
	short end;
}rt_node;
/*初始化*/
struct rt_node* Init_rdx_tree();
/*插入*/
struct rt_node* Insert(struct rt_node* node, struct Bits* b);
/*搜索*/
int Search(struct rt_node* node, struct Bits* b);
#endif 
