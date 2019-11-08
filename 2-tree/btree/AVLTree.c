#include <stdio.h>
#include <stdlib.h>
#include "avltree.h"
#include < assert.h >
#include<string.h>
#define HEIGHT(node) ((node==NULL) ? 0 :(((avlnode *)(node))->height))
#define MAX(a,b) ((a)>(b)?(a):(b))

/*****动态内存******/
void* bupt_malloc(size_t size) {
	if (size <= 0)
	{
		return NULL;
	}

	global_stats.mem += size;
	return malloc(size);
}
/*****字符比较******/
int byte_cmp(char a, char b) {
	global_stats.cmpnum++;
	return a - b;
}
/*****字符串对比******/
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
/*****当前节点高度******/
int getNode_height(avlnode* node)
{
	return HEIGHT(node);
}
/*****创建节点******/
avlnode* create_node(char key[65], avlnode* left, avlnode* right)
{
	avlnode* node = (avlnode*)bupt_malloc(sizeof(avlnode));
	if (node == NULL)
	{
		printf("创建结点失败");
		return NULL;
	}
	strcpy(node->key,key);
	node->left = left;
	node->right = right;
	node->height = 0;
	global_stats.node_num++;
	return node;
}

/*LL旋转*/
static avltree left_left_rotation(avltree tree)
{
	avlnode* k2 = tree->left;
	tree->left = k2->right;
	k2->right = tree;
	//所有旋转操作后要重新调整树的高度
	tree->height = MAX(getNode_height(tree->left), getNode_height(tree->right)) + 1;
	k2->height = MAX(getNode_height(k2->left), getNode_height(k2->right)) + 1;
	return k2;
}

/*RR旋转*/
static avltree right_right_rotation(avltree tree)
{
	avlnode* k3 = tree->right;
	tree->right = k3->left;
	k3->left = tree;
	tree->height = MAX(getNode_height(tree->left), getNode_height(tree->right)) + 1;
	k3->height = MAX(getNode_height(k3->left), getNode_height(k3->right)) + 1;
	return k3;
}

/*LR旋转*/
static avltree left_right_rotation(avltree tree)
{
	tree->left = right_right_rotation(tree->left);
	tree = left_left_rotation(tree);
	return tree;
}

/*RL旋转*/
static avltree right_left_rotation(avltree tree)
{
	tree->right = left_left_rotation(tree->right);
	tree = right_right_rotation(tree);
	return tree;
}


 /*****插入节点******/
avltree  avltree_insertNode(avltree tree, char key[65])
{
	if (tree == NULL)
	{
		avlnode* node = create_node(key, NULL, NULL);
		tree = node;
	}
	else if (bupt_strcmp(key ,tree->key)<0)//在左子树中插入结点
	{
		tree->left = avltree_insertNode(tree->left, key);//递归寻找插入节点的位置
		//插入节点后可能引起二叉树的不平衡，所以要在此进行判断
		if (HEIGHT(tree->left) - HEIGHT(tree->right) == 2)
		{
			//在这儿判断是LL还是LR
			if (tree->left){
				if (bupt_strcmp(key , tree->left->key)<0)
				{
					//LL旋转
					tree = left_left_rotation(tree);

				}
				else
				{
					//LR旋转
					tree = left_right_rotation(tree);
				}
			}
		}
	}
	else if (bupt_strcmp(key ,tree->key)>0)//在右子树中插入结点
	{
		tree->right = avltree_insertNode(tree->right, key);
		if (getNode_height(tree->right) - getNode_height(tree->left) == 2)
		{
			//RR旋转
			if (bupt_strcmp(key,tree->right->key)>0)
			{
				tree = right_right_rotation(tree);
			}
			else
			{
				//RL旋转
				tree = right_left_rotation(tree);
			}
		}
	}
	//重新调整二叉树的深度
	tree->height = MAX(getNode_height(tree->left), getNode_height(tree->right)) + 1;

	return tree;
}

/*****搜索节点******/
int search_node(avltree tree, char key[65])
{
	if (tree == NULL) {
		return 0;
	}
	int cmp = bupt_strcmp(tree->key, key);
	if (cmp==0)
	{
		return 1;
	}
	else if (cmp > 0)
	{
		search_node(tree->left, key);
	}
	else
	{
		search_node(tree->right, key);
	}
}


