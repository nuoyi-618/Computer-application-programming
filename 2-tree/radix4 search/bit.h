#pragma once
#ifndef RADIX4_SEARCH_BITS__H
#define RADIX4_SEARCH_BITS__H
#define RADIX_NODE_SIZE 4

struct Bits {
	char* value;
	int start;
	int length;
};

struct Bits* init_bits(char* str);
int bits_get(struct Bits* bits, int postion);
int branch(struct Bits* bits, int postion);
#endif 