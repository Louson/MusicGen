#include <stdio.h>

#include "read.h"

int
read_int(FILE* file)
{
	unsigned char s[4];
	int res, i;
	fread((void*)s, sizeof(char), 4, file);
	res = s[0];
	for (i = 1 ; i<4 ; i++) {
	res = (res << 8) + s[i];
}
	return res;
}

short
read_short(FILE* file)
{
	unsigned char s[4];
	int res, i;
	fread((void*)s, sizeof(char), 2, file);
	res = s[0];
	for (i = 1 ; i<2 ; i++) {
		res = (res << 8) + s[i];
	}
	return res;
}
