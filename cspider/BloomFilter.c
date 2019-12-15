#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "cspider.h"
#include "hash_functions.h"
int BF_Size = BF_SIZE;
int CHAR_BITS_NUM = 8;
int HASH_NUM = 10;
double PRM_EXP_FAIL_RATE = 0.00001;  
int PRM_STR_NUM_TO_STORE = 350000; 
int PRM_BITS_NUM = 1024 * 1024 * 3 * 8;
int PRM_HASH_NUM = 10;


unsigned int(*Hash_List[])(char*, unsigned int) = {
	BKDRHash, RSHash, JSHash, PJWHash, ELFHash,
	SDBMHash, DJBHash, DEKHash, BPHash, FNVHash,
	APHash, MyHash };
int Bit_Operators[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };

void init_bf(BF *bf, int length) {
	for (int i = 0; i < length; i++) {
		bf->bits[i] &= 0;
	}
}

void set_bf_bits(BF *bf, char* p) {
	for (int i = 0; i < HASH_NUM; i++) {
		int index = Hash_List[i](p, strlen(p)) % (BF_Size * CHAR_BITS_NUM);
		bf->bits[index / CHAR_BITS_NUM] |= Bit_Operators[index % CHAR_BITS_NUM];
	}
	return;
}

void free_bf(BF *bf) {
	free(bf->bits);
	free(bf);
	return;
}

int check_bf_bits(BF *bf, char* p) {
	for (int i = 0; i < HASH_NUM; i++) {
		int index = Hash_List[i](p, strlen(p)) % (BF_Size * CHAR_BITS_NUM);
		if(((*bf).bits[index / CHAR_BITS_NUM] & Bit_Operators[index % CHAR_BITS_NUM]) == 0) {
			return 0;
		}
	}
	return 1;
}

void bf_params(void) {
	int n, m, k;
	double f, exp_f, exp_m, exp_k;
	f = PRM_EXP_FAIL_RATE;
	n = PRM_STR_NUM_TO_STORE;
	m = PRM_BITS_NUM;
	k = PRM_HASH_NUM;

	exp_m = (double)n * 1.44 * log(1 / f);
	exp_k = 0.693 * (double)m / (double)n;
	exp_f = pow((double)1 - exp(-(double)k * (double)n / (double)m), k);
	return;
}
