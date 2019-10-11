#pragma once
#include<stdint.h>
#include<stddef.h>
int mem ;//ͳ���ڴ�
int cmpnum;//ͳ�ƱȽϴ���
typedef struct BitMap {
	uint64_t* data;
	size_t capacity;
}BitMap;

/******��ʼ��*****/
void BitMapInit(BitMap* bm, size_t capacity);

/*****ƫ��λ��1******/
void BitMapSet(BitMap* bm, size_t index);

/*****ƫ��λ��0******/
void BitMapUnSet(BitMap* bm, size_t index);

/*****�鿴ƫ��λ��ֵ******/
int BitMapTest(BitMap* bm, size_t index);

//void* bupt_malloc(size_t size);

/****�ͷ�******/
void BitMapDestory(BitMap* bm);
