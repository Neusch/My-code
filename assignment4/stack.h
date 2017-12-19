# ifndef STACK_H
# define STACK_H

# include <stdint.h>
# include <stdbool.h>
# include "huffman.h"

// Stack header file provided by Darrell Long

typedef treeNode item;

typedef struct stack
{
	uint32_t size; // Full size of the stack
	uint32_t top; // Index of the top of the stack one place above the most recent item
	item **entries; // Array to hold the items
} stack;

stack *newStack(void); // Construct a stack

void delStack(stack *); // Delete a stack

item *pop(stack *); // Returns the top item 
void push(stack *, item *); // Adds an item to the top of the stack

bool empty(stack *); // Check if the stack is empty
bool full(stack *); // Is the stack full?
bool moreThanOne(stack *s); // is there only one element on the stack?
# endif

