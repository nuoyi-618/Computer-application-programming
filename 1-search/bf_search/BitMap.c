#define _CRT_SECURE_NO_WARNINGS 1
#include"BitMap.h"
#include<stdlib.h>
#include<stdint.h>
#include<stddef.h>
#include<assert.h>
#include<string.h>
mem = 0;
cmpnum = 0;
/******ͳ���ڴ�*****/
void* bupt_malloc(size_t size) {
	if (size <= 0)
	{
		return NULL;
	}

	mem += size;
	return malloc(size);
}
/******���ݴ�С*****/
size_t DataSize(size_t capacity) {
	return capacity / (sizeof(uint64_t) * 8) + 1;//sizeof(uint64_t)��8���ֽ�,capacity��ʾ��Ҫ�洢����������
}

/******��ʼ��*****/
void BitMapInit(BitMap* bm, size_t capacity) {
	if (bm == NULL) {
		return;
	}
	bm->capacity = capacity;
	size_t size = DataSize(capacity);//�õ���size��ʾ��Ҫ�����ģ���λ�Ĵ洢�ռ�ĸ���
	bm->data = (uint64_t*)bupt_malloc(sizeof(uint64_t) * size);
	memset(bm->data, 0, sizeof(uint64_t) * size);//��s�е�ǰλ�ú����n���ֽ� ��typedef unsigned int size_t ���� ch �滻������ s
}

size_t GetOffset(size_t index, size_t* n, size_t* offset) {
	assert(n);
	assert(offset);
	*n = index / (sizeof(uint64_t) * 8);//��n��64λ�ռ�
	*offset = index % (sizeof(uint64_t) * 8);//ƫ����
}

/*****ƫ��λ��1******/
void BitMapSet(BitMap* bm, size_t index) {
	if (bm == NULL) {
		return;
	}
	if (index >= bm->capacity) {//�����������ܴ洢��������������
		return;
	}
	size_t n = 0;
	size_t offset = 0;
	GetOffset(index, &n, &offset);
	bm->data[n] |= (1lu << offset);//��1�ƶ���ָ��λ��
}

/*****ƫ��λ��0******/
void BitMapUnSet(BitMap* bm, size_t index) {
	if (bm == NULL) {
		return;
	}
	size_t n, offset;
	GetOffset(index, &n, &offset);
	bm->data[n] &= ~(1lu << offset);//��ƫ��1���ֵ���������㣬����
}


/*****�鿴ƫ��λ��ֵ******/
int BitMapTest(BitMap* bm, size_t index) {//����indexΪ1����Ϊ0�������1���ͷ���1�����򷵻�0
	if (bm == NULL) {
		return;
	}
	size_t n, offset;
	GetOffset(index, &n, &offset);
	int i = bm->data[n] & (1lu << offset);//��ƫ��1���ֵ����������
	cmpnum ++;
	return i != 0 ? 1 : 0;
}

/******�ͷ�*****/
void BitMapDestory(BitMap* bm) {
	if (bm == NULL) {
		return;
	}
	free(bm->data);
	bm->capacity = 0;
}

