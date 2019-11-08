#include "bit.h"

struct Bits* init_bits(char* str) {
	struct Bits* bits = malloc(sizeof(struct Bits));
	bits->value = str;
	bits->start = 0;
	//bits数组需要的长度
	bits->length = strlen(bits->value) * 8;
	return bits;
}
/*判断0/1*/
int bits_get(struct Bits* bits, int postion) {
	char* str = bits->value;
	if (postion >= bits->length) return -1;
	else {
		return (str[postion / 8] >> (postion % 8)) & 1;
	}
}
/*分支判断*/
int branch(struct Bits* bits, int postion) {
	int b = 0;
	int offset = postion;
	if (postion >= bits->length) return -1;
	else {
		if (bits_get(bits, postion) == 0) {
			if (bits_get(bits, postion + 1) == 1){
				b=1;
			}
		}
		else if (bits_get(bits, postion + 1) == 0) {
			b = 2;
		}
		else
		{
			b = 3;
		};
	}

	return b;
}