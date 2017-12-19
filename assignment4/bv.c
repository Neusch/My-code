#include "bv.h"

bitV *newVec(uint32_t len)
{
	bitV *vec = (bitV *) malloc(sizeof(bitV));
	vec->l = len;
	vec->v = (uint8_t *) calloc(len / 8 + 1, sizeof(uint8_t));
	return vec;
}

void delVec(bitV *vec)
{
	free(vec->v);
	free(vec);
}

void oneVec(bitV *vec)
{
	memset(vec->v, 0xFF, vec->l / 8 + 1);
}

void setBit(bitV *vec, uint32_t pos)
{
	vec->v[pos / 8] |= (1 << (pos % 8));
}

void clrBit(bitV *vec, uint32_t pos)
{
	vec->v[pos / 8] &= ~(1 << (pos % 8));
}

uint8_t valBit(bitV *vec, uint32_t pos)
{
	return vec->v[pos / 8] & (1 << (pos % 8));
}

uint32_t lenVec(bitV *vec)
{
	return vec->l;
}

uint32_t sizeVec(bitV *vec)
{
	return vec->l / 8 + 1;
}

