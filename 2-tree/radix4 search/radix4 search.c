#define _CRT_SECURE_NO_WARNINGS 1
#include<assert.h>
#include "radix4tree.h"

void* bupt_malloc(size_t size) {
	if (size <= 0) {
		return NULL;
	}
	global_stats.mem += size;
	return (malloc(size));
}
/*新建节点*/
struct rt_node* new_node_init(struct rt_node* node) {
	node->parent = NULL;
	for (int i = 0; i < RADIX_NODE_SIZE; i++)
	{
		node->child[i] = NULL;
	}
	node->end = 0;
	return node;
}
/*初始化*/
struct rt_node* Init_rdx_tree() {
	struct rt_node* headnode = bupt_malloc(sizeof(struct rt_node));
	headnode = new_node_init(headnode);//头节点
	return headnode;
}
/*插入*/
struct rt_node* Insert(struct rt_node* node, struct Bits* b) {
	if (b->length <= 0) return node;
	if (b->start <= b->length - 2) {
		int x = branch(b, b->start);
		//子节点为空时
		if (node->child[x] == NULL) {
			struct rt_node* new_node = bupt_malloc(sizeof(struct rt_node));
			global_stats.node_num++;
			new_node = new_node_init(new_node);
			new_node->parent = node;
			node->child[x] = new_node;
			//bits的读取位置移动2位
			b->start += 2;
			//从新节点开始插入
			Insert(node->child[x], b);
		}
		//子节点存在时，二进制字符串移位，向下寻找下一节点
		else { 
			b->start += 2;
			Insert(node->child[x], b);
		}
	}
	else {
		node->end = 1;
	}
	return node;
}
/*搜索*/
int Search(struct rt_node* node, struct Bits* b) {
	if (b->length <= 0) return 0;
	int x = branch(b, b->start);
	if (b->start <= b->length - 2) {
		global_stats.cmp_num++;
		if (node->child[x] == NULL) return 0;
		else {
			b->start += 2;
			Search(node->child[x], b);
		}
	}
	else {
		if (node->end == 1) return 1;
		else return 0;
	};
}