
# ifndef HASH_H
# define HASH_H

# include <stdint.h>
# include "linkedList.h"
# include "bloomFilter.h"

extern uint32_t average;

typedef struct hashTable
{
	uint32_t salt[4]; // Salt
	uint32_t length;
	listNode **h; // Array of pointers to linked lists
}hashTable;

int realLength(int l);

uint32_t hash(uint32_t salt[], const char *key); // Used for hashing the bloom filter

hashTable *newHT(uint32_t , uint32_t []); // Create a new hash table

void delHT(hashTable *); // Delete a hashTable

listNode *findHT(hashTable *, char *); // Find an item in the hash table

void insertHT(hashTable *, char *, char *); // Insert an item into the hash table

void printHT(const hashTable *); // print hashTable 
# endif