# ifndef _HUFFMAN_H
# define _HUFFMAN_H
# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <assert.h>
# include "code.h"

typedef struct DAH treeNode;

struct DAH
{
	uint8_t symbol;
	uint64_t count;
	bool leaf;
	treeNode *left, *right;
};

# include "stack.h"

// New node , with symbols , leaf or not , a count associated with it
treeNode *newNode(uint8_t s, bool l, uint64_t c);

// Dump a Huffman tree onto a file
void dumpTree(treeNode *t, FILE *file);

// Build a tree from the saved tree
treeNode *loadTree(uint8_t savedTree[], uint16_t treeBytes);

// Step through a tree following the code
int32_t stepTree(treeNode *root ,treeNode **t, uint32_t code);

// Parse a Huffman tree to build codes
void buildCode(treeNode *t, code s, code table[256]);

void delTree(treeNode *t);

static inline void delNode(treeNode *h) { free(h); return; }

static inline int8_t compare(treeNode *l, treeNode *r)
{
	return l->count - r->count; // l < r if negative , l = r if 0 , ...
}

treeNode *join(treeNode *l, treeNode *r); // Join two subtrees

void printTree(treeNode *t,int depth);
#endif
