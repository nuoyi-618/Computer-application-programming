#pragma once

#include<stddef.h>
#include"BitMap.h"

#define HASHFUNCMAXSIZE 9
#define BitMapCapacity 35000000


/******哈希函数*******/
typedef size_t(*HashFunc)(const char*);

typedef struct BloomFilter {
	BitMap bitmap;
	HashFunc hash_func[HASHFUNCMAXSIZE];
}BloomFilter;



/*****初始化布隆过滤器*****/
void BloomFilterInit(BloomFilter* bf);

/*****插值*****/
void BloomFilterInsert(BloomFilter* bf, const char* str);
/*****检测值是否存在******/
int BloomFilterExit(BloomFilter* bf, const char* str);
/*****释放*******/
void BloomFilterDestory(BloomFilter* bf);

