#pragma once

#ifndef BPlusTree_h
#define BPlusTree_h

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define M (4)
#define LIMIT_M_2 (M % 2 ? (M + 1)/2 : M/2)
struct{
	long node_num;//�ڵ���
	long cmpnum;//�ַ��Ƚϴ���
	long mem;//�ڴ��С
	long words_n;//�ܴ���
	long right;//��ȷ�ĸ���
}global_stats;

typedef struct BPlusNode* BPlusTree, * Position;
typedef char* KeyType;
struct BPlusNode {
	int KeyNum;
	KeyType Key[M + 1];
	BPlusTree Children[M + 1];
	BPlusTree Next;
}node;

/* ��ʼ�� */
extern BPlusTree Initialize();
/* ���� */
extern BPlusTree Insert(BPlusTree T, KeyType Key);
/* ���� */
extern BPlusTree Destroy(BPlusTree T);
/* �����ڵ� */
extern void Travel(BPlusTree T);
/* ������Ҷ�ڵ������ */
extern void TravelData(BPlusTree T);
/*���ҽڵ�*/
int Search(BPlusTree T, KeyType Key);
#endif BPlusTree_h 