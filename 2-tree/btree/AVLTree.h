#pragma once
#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED
struct stats {
	long node_num;//节点数
	long cmpnum;//字符比较次数
	long mem;//内存大小
	long words_n;//总词数
	long right;//正确的个数
}global_stats;

typedef struct node
{
	char key[65];
	struct node* left;
	struct node* right;
	int height;//当前结点深度
} avlnode, * avltree;

int getNode_height(avlnode* node);//获取当前结点的深度

avlnode* create_node(char key[65], avlnode* left, avlnode* right);//创建结点

avltree  avltree_insertNode(avltree tree, char key[65]);//向树中插入结点

int search_node(avltree tree, char key[65]);//根据key 的值搜索结点

#endif 
