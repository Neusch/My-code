// Header file for a bloom filter provided by Darrell Long

# ifndef BF_H
# define BF_H

# include <stdlib.h>
# include <stdint.h>
# include <stdio.h>
# include <string.h>
# include "aes.h"
# include "bv.h"
# include "hash.h"

typedef struct bloomF {
	uint8_t *vector;//bitV *vector;
	uint32_t length;
	uint32_t salt[4];
}bloomF;

uint32_t hashBloomF(bloomF *bf,char *data);

bloomF *newBF(uint32_t length, uint32_t b[]);

void delBF(bloomF *v);

uint32_t valBF(bloomF *x, uint32_t k);

uint32_t lenBF(bloomF *x);

uint32_t countBF(bloomF *b);

void setBF(bloomF *x, char * key);

void clrBF(bloomF *x, char *key);

uint32_t memBF(bloomF *x, char *key);

void printBF(bloomF *x); 
 
/*
 int realLength1(int l) 
{ 
	return 16 * (l / 16 + (l % 16 ? 1 : 0));
}

static inline uint32_t hashBF(uint32_t salt[],char *key) // Each function has its own hash function , determined by the salt
{
	uint32_t output [4] = { 0x0 }; 
	uint32_t sum = 0x0; 
	int keyL = strlen(key);
	uint8_t *realKey = (uint8_t *) calloc(realLength1(keyL), sizeof(uint8_t));
	memcpy(realKey , key , keyL);
	for (int i = 0; i < realLength1(keyL); i += 16) 
	{ 
		AES128_ECB_encrypt (( uint8_t *) salt, // Salt
				(uint8_t *) realKey + i, // Input
				(uint8_t *) output); // Output
		sum ^= output [0] ^ output [1] ^ output [2] ^ output [3]; 
	}
	free(realKey);
	return sum; 
}
static inline bloomF *newBF(uint32_t length, uint32_t b[]) // Create a new Bloom Filter of a given length and hash function
{
	bloomF *newBloomFilter;
	newBloomFilter = malloc(sizeof(bloomF));
	
	newBloomFilter->vector = calloc((length / 8) + 1, sizeof (uint8_t));
	newBloomFilter->length = length;
	for (uint8_t i = 0;i < 4;i++)
	{
		newBloomFilter->salt[i] = b[i];
	}
	return newBloomFilter;
}
static inline void delBF(bloomF *v) // Delete a Bloom filter
{
	free(v->vector);//delVec(v->vector);
	free(v);
	return;	
}
static inline uint32_t valBF(bloomF *x, uint32_t k) // Return the value of position k in the Bloom filter
{
	uint32_t temp;
	temp = x->vector[(k / 8)] & ((0x1 << (k % 8)) & 0xFF);// Get the value in the bit vector
	if (temp)
	{
		temp = 1;
	}
	else 
	{
		temp = 0;
	}
	return temp;
}
static inline uint32_t lenBF(bloomF *x) // Return the length of the bloom filter
{
	return x->length;
}

static inline uint32_t countBF(bloomF *b) // Count bits in the Bloom filter
{
	uint32_t numBits = 50000;
	if (b) {}
	//uint16_t j;
	  for (uint32_t i = 0;i < b->length;i++)
	{
		for (j = 1;j <= 255;j = j << 1)
		{
			if (b->vector[i] & j)
			{
				numBits++;
			}
		}
	} 
	return numBits; 
}

static inline void setBF(bloomF *x, char * key) // Set an entry in the Bloom filter
{
	uint32_t temp;
	temp = hashBF(x->salt,key) % x->length;
	//printf("Hash2: %u\n",temp);
	x->vector[(temp / 8)] |= 0x1 << (temp % 8);
	return;
}

static inline void clrBF(bloomF *x, char *key) // Clear an entry in the Bloom filter
{
	uint32_t temp;
	temp = hashBF(x->salt,key) % x->length;
	x->vector[(temp / 8)] &= ~(0x1 << (temp % 8)); 
	return;
}

static inline uint32_t memBF(bloomF *x, char *key) // Check membership in the Bloom filter
{
	uint32_t temp,temp2;
	temp = hashBF(x->salt,key) % x->length;
	temp2 = (valBF(x,temp)); // Return a one if something is already there or a zero if something isnt there
	return temp2; 
}

static inline void printBF(bloomF *x) // Print BF
{
	printf("\n");
	for (uint32_t i = x->length - 1;i > 0;i--)
	{
		printf("%X",x->vector[i]);
	}
	return;
} */

# endif // BF_H
