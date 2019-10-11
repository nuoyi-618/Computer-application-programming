#define _CRT_SECURE_NO_WARNINGS 1
#include"BloomFilter.h"
#include"BitMap.h"
#include<assert.h>


/******哈希函数*******/
unsigned int RSHash(const char* str, unsigned int length)
{
	unsigned int b = 378551;
	unsigned int a = 63689;
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = hash * a + (*str);
		a = a * b;
	}

	return hash;
}

unsigned int JSHash(const char* str, unsigned int length)
{
	unsigned int hash = 1315423911;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash ^= ((hash << 5) + (*str) + (hash >> 2));
	}

	return hash;
}
unsigned int PJWHash(const char* str, unsigned int length)
{
	const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
	const unsigned int ThreeQuarters = (unsigned int)((BitsInUnsignedInt * 3) / 4);
	const unsigned int OneEighth = (unsigned int)(BitsInUnsignedInt / 8);
	const unsigned int HighBits =
		(unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
	unsigned int hash = 0;
	unsigned int test = 0;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = (hash << OneEighth) + (*str);

		if ((test = hash & HighBits) != 0)
		{
			hash = ((hash ^ (test >> ThreeQuarters))& (~HighBits));
		}
	}

	return hash;
}
unsigned int ELFHash(const char* str, unsigned int length)
{
	unsigned int hash = 0;
	unsigned int x = 0;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = (hash << 4) + (*str);

		if ((x = hash & 0xF0000000L) != 0)
		{
			hash ^= (x >> 24);
		}

		hash &= ~x;
	}

	return hash;
}
unsigned int BKDRHash(const char* str, unsigned int length)
{
	unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = (hash * seed) + (*str);
	}

	return hash;
}
unsigned int SDBMHash(const char* str, unsigned int length)
{
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = (*str) + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}
unsigned int DJBHash(const char* str, unsigned int length)
{
	unsigned int hash = 5381;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = ((hash << 5) + hash) + (*str);
	}

	return hash;
}
unsigned int DEKHash(const char* str, unsigned int length)
{
	unsigned int hash = length;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
	}

	return hash;
}
unsigned int APHash(const char* str, unsigned int length)
{
	unsigned int hash = 0xAAAAAAAA;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3)) :
			(~((hash << 11) + ((*str) ^ (hash >> 5))));
	}

	return hash;
}
unsigned int BPHash(const char* str, unsigned int len)
{
	unsigned int hash = 0;
	unsigned int i = 0;
	for (i = 0; i < len; str++, i++)
	{
		hash = hash << 7 ^ (*str);
	}

	return hash;
}
unsigned int FNVHash(const char* str, unsigned int len)
{
	const unsigned int fnv_prime = 0x811C9DC5;
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i < len; str++, i++)
	{
		hash *= fnv_prime;
		hash ^= (*str);
	}

	return hash;
}
/*****初始化布隆过滤器*****/
void BloomFilterInit(BloomFilter* bf) {
	if (bf == NULL) {
		return;
	}
	BitMapInit(&bf->bitmap, BitMapCapacity);//初始化位图
	//初始初始化哈希函数
	bf->hash_func[0] = RSHash;
	bf->hash_func[1] = JSHash;
	bf->hash_func[2] = PJWHash;
	bf->hash_func[3] = ELFHash;
	bf->hash_func[4] = BKDRHash;
	bf->hash_func[5] = SDBMHash;
	bf->hash_func[6] = DJBHash;
	bf->hash_func[7] = DEKHash;
	bf->hash_func[8] = APHash;	
	//bf->hash_func[9] = BPHash;
	//bf->hash_func[10] = FNVHash;
}

/*****插值*****/
void BloomFilterInsert(BloomFilter* bf, const char* str,int len) {
	assert(bf);
	assert(str);
	int i = 0;
	for (; i < HASHFUNCMAXSIZE; ++i) {
		size_t offset = bf->hash_func[i](str,len) % BitMapCapacity;
		BitMapSet(&bf->bitmap, offset);
	}
	return;
}

/*****检测值是否存在******/
int BloomFilterExit(BloomFilter* bf, const char* str,int len) {
	if (bf == NULL || str == NULL) {
		return 0;
	}
	int i = 0;
	for (; i < HASHFUNCMAXSIZE; ++i) {
		size_t offset = bf->hash_func[i](str,len) % BitMapCapacity;
		int n = BitMapTest(&bf->bitmap, offset);
		if (n == 0) {//有一位不为1则不存在
			return 0;
		}
	}
	return 1;
}
/*****释放*******/
void BloomFilterDestory(BloomFilter* bf) {
	if (bf == NULL) {
		return;
	}
	BitMapDestory(&bf->bitmap);
	int i = 0;
	for (; i < HASHFUNCMAXSIZE; ++i) {
		bf->hash_func[i] = NULL;
	}
}

