#pragma once

#include<stddef.h>
#include"BitMap.h"

#define HASHFUNCMAXSIZE 9
#define BitMapCapacity 35000000


/******��ϣ����*******/
typedef size_t(*HashFunc)(const char*);

typedef struct BloomFilter {
	BitMap bitmap;
	HashFunc hash_func[HASHFUNCMAXSIZE];
}BloomFilter;



/*****��ʼ����¡������*****/
void BloomFilterInit(BloomFilter* bf);

/*****��ֵ*****/
void BloomFilterInsert(BloomFilter* bf, const char* str);
/*****���ֵ�Ƿ����******/
int BloomFilterExit(BloomFilter* bf, const char* str);
/*****�ͷ�*******/
void BloomFilterDestory(BloomFilter* bf);

