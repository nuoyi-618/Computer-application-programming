#pragma once
#include<stdint.h>
#include<stddef.h>
int mem ;//统计内存
int cmpnum;//统计比较次数
typedef struct BitMap {
	uint64_t* data;
	size_t capacity;
}BitMap;

/******初始化*****/
void BitMapInit(BitMap* bm, size_t capacity);

/*****偏移位标1******/
void BitMapSet(BitMap* bm, size_t index);

/*****偏移位标0******/
void BitMapUnSet(BitMap* bm, size_t index);

/*****查看偏移位数值******/
int BitMapTest(BitMap* bm, size_t index);

//void* bupt_malloc(size_t size);

/****释放******/
void BitMapDestory(BitMap* bm);
