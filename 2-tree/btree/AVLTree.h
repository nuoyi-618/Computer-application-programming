#pragma once
#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED
struct stats {
	long node_num;//�ڵ���
	long cmpnum;//�ַ��Ƚϴ���
	long mem;//�ڴ��С
	long words_n;//�ܴ���
	long right;//��ȷ�ĸ���
}global_stats;

typedef struct node
{
	char key[65];
	struct node* left;
	struct node* right;
	int height;//��ǰ������
} avlnode, * avltree;

int getNode_height(avlnode* node);//��ȡ��ǰ�������

avlnode* create_node(char key[65], avlnode* left, avlnode* right);//�������

avltree  avltree_insertNode(avltree tree, char key[65]);//�����в�����

int search_node(avltree tree, char key[65]);//����key ��ֵ�������

#endif 
