# include <stdlib.h>
# include <stdint.h>
# include <string.h>
# include "aes.h"
# include "linkedList.h"
# include "hash.h"
# include "bloomFilter.h"

// ****** Hash code given by Darrell Long ******* //
int realLength(int l) 
{ 
	return 16 * (l / 16 + (l % 16 ? 1 : 0));
} 

uint32_t hash(uint32_t salt[], const char *key)
{
	uint32_t output [4] = { 0x0 }; 
	uint32_t sum = 0x0; 
	int keyL = strlen(key);
	uint8_t *realKey = (uint8_t *) calloc(realLength(keyL), sizeof(uint8_t));
	memcpy(realKey , key , keyL);
	for (int i = 0; i < realLength(keyL); i += 16) 
	{ 
		AES128_ECB_encrypt (( uint8_t *) salt, // Salt
				(uint8_t *) realKey + i, // Input
				(uint8_t *) output); // Output
		sum ^= output [0] ^ output [1] ^ output [2] ^ output [3]; 
	}
	free(realKey);
	return sum; 
}
// ************ end of hash code given by darrell long *********** // 

hashTable *newHT(uint32_t length, uint32_t salt[]) // Create a new hash table
{
	hashTable *temp;
	temp = malloc(sizeof(hashTable));
	
	if (temp == NULL)
	{
		return NULL;
	}
	else
	{
		for (uint8_t i = 0;i < 4;i++)
		{
			temp->salt[i] = salt[i];
		}
		temp->length = length;
		temp->h = malloc(sizeof(listNode) * length);
	}
	return temp;
}

void delHT(hashTable *hT) // Delete a hashTable
{
	for (uint32_t i = 0;i < hT->length;i++)
	{
		if (hT->h[i] != NULL)
		{
			delLinkedList(hT->h[i]);
		}
	}
	free(hT->h);
	free(hT);
	return;
}
listNode *findHT(hashTable *hT, char *data) // Find an item in the hash table
{
	uint32_t temp;
	listNode *tempNode;
	temp = hash(hT->salt,data) % hT->length;

	if (hT->h[temp] != NULL)
	{
		average++; // Add one for each time a seek through the hash table happens
		tempNode = findLinkedList(&hT->h[temp],data);
		return tempNode;
	}
	else
	{
		return NULL;
	}
}	

void insertHT(hashTable *hT, char *data, char *data2) // Insert an item into the hash table
{
	uint32_t temp;
	temp = hash(hT->salt,data) % hT->length;
	
	listNode *tempList;
	tempList = findHT(hT,data);
	if (tempList != NULL)
	{
		// If the oldspeak word is already in the list, then just replace the newspeak word
		insertLinkedList(&hT->h[temp],data,data2);
		return;
	}	
	
	if (hT->h[temp] == NULL)
	{
		hT->h[temp] = newNode(data,data2);
	} 
	else
	{
		hT->h[temp] = insertLinkedList(&hT->h[temp],data,data2);
	}
	return;
}

void printHT(const hashTable *hT)
{
	for (uint32_t i = 0;i < hT->length;i++)
	{
		if (hT->h[i] == NULL)
		{
			continue;
		}
		else
		{
			printLinkedList(hT->h[i]);
		}
	}
	return;
} 