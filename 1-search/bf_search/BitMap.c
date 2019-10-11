#define _CRT_SECURE_NO_WARNINGS 1
#include"BitMap.h"
#include<stdlib.h>
#include<stdint.h>
#include<stddef.h>
#include<assert.h>
#include<string.h>
mem = 0;
cmpnum = 0;
/******统计内存*****/
void* bupt_malloc(size_t size) {
	if (size <= 0)
	{
		return NULL;
	}

	mem += size;
	return malloc(size);
}
/******数据大小*****/
size_t DataSize(size_t capacity) {
	return capacity / (sizeof(uint64_t) * 8) + 1;//sizeof(uint64_t)是8个字节,capacity表示需要存储的数据类型
}

/******初始化*****/
void BitMapInit(BitMap* bm, size_t capacity) {
	if (bm == NULL) {
		return;
	}
	bm->capacity = capacity;
	size_t size = DataSize(capacity);//得到的size表示需要容量的６４位的存储空间的个数
	bm->data = (uint64_t*)bupt_malloc(sizeof(uint64_t) * size);
	memset(bm->data, 0, sizeof(uint64_t) * size);//将s中当前位置后面的n个字节 （typedef unsigned int size_t ）用 ch 替换并返回 s
}

size_t GetOffset(size_t index, size_t* n, size_t* offset) {
	assert(n);
	assert(offset);
	*n = index / (sizeof(uint64_t) * 8);//第n个64位空间
	*offset = index % (sizeof(uint64_t) * 8);//偏移量
}

/*****偏移位标1******/
void BitMapSet(BitMap* bm, size_t index) {
	if (bm == NULL) {
		return;
	}
	if (index >= bm->capacity) {//超过现在所能存储的数据容量返回
		return;
	}
	size_t n = 0;
	size_t offset = 0;
	GetOffset(index, &n, &offset);
	bm->data[n] |= (1lu << offset);//将1移动到指定位置
}

/*****偏移位标0******/
void BitMapUnSet(BitMap* bm, size_t index) {
	if (bm == NULL) {
		return;
	}
	size_t n, offset;
	GetOffset(index, &n, &offset);
	bm->data[n] &= ~(1lu << offset);//与偏移1后的值进行与运算，标零
}


/*****查看偏移位数值******/
int BitMapTest(BitMap* bm, size_t index) {//测试index为1还是为0，如果是1，就返回1，否则返回0
	if (bm == NULL) {
		return;
	}
	size_t n, offset;
	GetOffset(index, &n, &offset);
	int i = bm->data[n] & (1lu << offset);//与偏移1后的值进行与运算
	cmpnum ++;
	return i != 0 ? 1 : 0;
}

/******释放*****/
void BitMapDestory(BitMap* bm) {
	if (bm == NULL) {
		return;
	}
	free(bm->data);
	bm->capacity = 0;
}

