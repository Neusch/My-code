
# include "bloomFilter.h"
# include "hash.h"
# include "bv.h"

uint32_t hashBloomF(bloomF *bf,char *data)
{
	uint32_t temp;
	temp = hash(bf->salt,data) % bf->length;
	return temp;
}

bloomF *newBF(uint32_t length, uint32_t b[])
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

void delBF(bloomF *v)
{
	free(v->vector); // Free the memory in the bloom filter
	free(v);
	return;
}

uint32_t valBF(bloomF *x, uint32_t k)
{
	uint32_t temp;
	temp = x->vector[(k / 8)] & ((0x1 << (k % 8)) & 0xFF);// Get the value in the bit vector
	if (temp)
	{
		temp = 1; // Return a one if there was a value there or a zero if there wasnt
	}
	else 
	{
		temp = 0;
	}
	return temp;	
}

uint32_t lenBF(bloomF *x)
{
	return x->length;
}

uint32_t countBF(bloomF *b)
{
	uint32_t numBits = 0;
	uint16_t j;
	for (uint32_t i = 0;i < (b->length/8);i++)
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

void setBF(bloomF *x, char *key)
{
	uint32_t temp;
	temp = hash(x->salt,key) % x->length;
	x->vector[(temp / 8)] |= 0x1 << (temp % 8);
	return;
}

void clrBF(bloomF *x, char *key)
{
	uint32_t temp;
	temp = hash(x->salt,key) % x->length;
	x->vector[(temp / 8)] &= ~(0x1 << (temp % 8)); 
	return;
}

uint32_t memBF(bloomF *x, char *key)
{
	uint32_t temp,temp2;
	temp = hash(x->salt,key) % x->length;
	temp2 = (valBF(x,temp)); // Return a one if something is already there or a zero if something isnt there
	return temp2; 
}

void printBF(bloomF *x)
{
	printf("\n");
	for (uint32_t i = x->length - 1;i > 0;i--)
	{
		printf("%X",x->vector[i]);
	}
	return;
}