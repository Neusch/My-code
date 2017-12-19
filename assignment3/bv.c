// Spencer Neuschmid

# include "bv.h"
# include <stdlib.h>
# include <stdio.h>

bitV *newVec(uint32_t length)
{
	bitV *tempVec;
	tempVec = malloc(sizeof (bitV));
	if (tempVec != NULL)
	{
		tempVec->v = calloc((length / 8) + 1, sizeof (uint8_t));
		tempVec->l = length;  
		return tempVec;	
	}
	else
	{
		return (NULL);
	}
}
// Delete a vector
void delVec(bitV *bitVector)
{
	free(bitVector);
	free(bitVector->v);
	return;	
}
// Create a vector of all 1s
void one(bitV *bitVector)
{
	int i;
	for(i = 0; i < (int) bitVector->l; i++)
	{
		bitVector->v[i] |= (0xFF); // Set all 8 bits to be 1
	}
} 
// Set a specified bit 
void setBit(bitV *bitVector, uint32_t bitNumber)
{
	bitVector->v[(bitNumber / 8)] |= 0x1 << (bitNumber % 8);
}
// Clear a specified bit
void clrBit(bitV *bitVector, uint32_t bitNumber)
{
	bitVector->v[(int) (bitNumber / 8)] &= ~(0x1 << (bitNumber % 8));
}
// Return the value of a specified bit
uint8_t valBit(bitV *bitVector, uint32_t bitNumber)
{
	if(bitVector->v[(int) (bitNumber / 8)] & ((0x1 << (bitNumber % 8)) & 0xFF))
	{		
		return 1;
	}
	else
	{
		return 0;
	}
}
// Return the length of the vector
uint32_t lenVec(bitV *bitVector)
{
	return bitVector->l; // 8 == sizeof(uint8_t)
}
