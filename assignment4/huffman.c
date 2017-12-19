# include "huffman.h"
# include <ctype.h>
# include <string.h>

// Create a new huffman tree node
treeNode *newNode(uint8_t s, bool l, uint64_t c)
{
	treeNode *n = malloc(sizeof(treeNode));
	n->symbol = s;
	n->count = c;
	n->leaf = l;
	n->left = n->right = NULL;
	
	return n;
}

// Dump a Huffman tree onto a file
void dumpTree(treeNode *t, FILE *file)
{
	if (t)
	{
		dumpTree(t->left, file); // Call dump tree recursively using post order traversal
		dumpTree(t->right, file);
		
		if (t->leaf)
		{
			fprintf(file, "L%c", t->symbol);
		}
		else
		{
			fprintf(file, "I");
		}
	}
}

// Build a tree from the saved tree
treeNode *loadTree(uint8_t savedTree[], uint16_t treeBytes)
{
	stack *s = newStack();
	
	for (uint16_t i = 0; i < treeBytes; i++) // This will recreate the tree based on the tree dumped into the file
	{
		if (savedTree[i] == 'L')
		{
			uint8_t c = savedTree[++i];
			
			treeNode *t = newNode(c, true, 0);
			push(s, t);
		}
		else if (savedTree[i] == 'I')
		{
			treeNode *right = pop(s);
			treeNode *left = pop(s);
			
			push(s, join(left, right));
		}
		else
		{
			printf("bad savedTree, got %c at %d\n", savedTree[i], i);
			exit(1);
		}
	}
	
	treeNode *ret = pop(s);
	assert(empty(s)); /* exits program if false */
	
	delStack(s);
	return ret;
}

// Step through a tree following the code
int32_t stepTree(treeNode *root, treeNode **t, uint32_t code)
{
	if (!code)
	{
		*t = (*t)->left;
	}
	else
	{
		*t = (*t)->right;
	}
	
	if ((*t)->leaf)
	{
		int32_t sym = (*t)->symbol;
		*t = root; /* reset it to root */
		return sym;
	}
	else
	{
		return -1; // Return a negative value to indicate an internal node
	}
}

// Parse a Huffman tree to build codes
void buildCode(treeNode *t, code s, code table[])
{
	if (t->leaf)
	{
		table[t->symbol] = s;
	}
	else
	{
		uint32_t k;
		pushCode(&s, 0);
		buildCode(t->left, s, table); // Push 0 when you go left
		popCode(&s, &k);
		pushCode(&s, 1);
		buildCode(t->right, s, table); // Push 1 when you go right
		popCode(&s, &k);
	}
}

// Free the data allocated to the tree
void delTree(treeNode *t)
{
	if (t)
	{
		delTree(t->left);
		delTree(t->right);
		
		delNode(t);
	}
}

// Join tree nodes together to make an internal node with children as the two inputs
treeNode *join(treeNode *l, treeNode *r)
{
	treeNode *t = newNode('$', false, l->count + r->count);
	t->left = l;
	t->right = r;
	return t;
}

// Print tree code, given by Darrell Long on Piazza

static void spaces(int num)
{
	for (int i = 0; i < num; i++)
	{
		putchar(' ');
	}
}

void printTree(treeNode *t, int depth)
{
	if (t && t->leaf)
	{
		if (isprint(t->symbol))
		{
			spaces(4 * depth);
			printf("%c (%lu)\n", t->symbol, t->count);
		}
		else
		{
			spaces(4 * depth);
			printf("0x%X (%lu)\n", t->symbol, t->count);
		}
	}
	else if (t)
	{
		spaces(4 * depth);
		printf("$ (%lu)\n", t->count);
		printTree(t->left, depth + 1);
		printTree(t->right, depth + 1);
	}
	return;
}
