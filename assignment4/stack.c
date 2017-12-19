# include "stack.h"
# include "huffman.h"
# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>

// Stack file provided by Darrell Long via Piazza, it is slightly modified to work for our purposes

# define MIN_STACK (10)

stack *newStack(void) // Construct a stack
{
	stack *s = (stack *) calloc(MIN_STACK, sizeof(stack));
	s->size = MIN_STACK;
	s->top = 0;
	s->entries = (item **) calloc(MIN_STACK,sizeof(item *));
	return s;
}

void delStack(stack *s) // Delete a stack
{
	free(s->entries);
	free(s);
}

item *pop(stack *s) // Returns the top item 
{
	if (s->top > 0)
	{
		s->top -= 1;
		return s->entries[s->top];
	}
	else 
	{
		return (item *) NULL;
	}
}

void push(stack *s,item *i) // Adds an item to the top of the stack
{
	if (s->top == s->size)
	{
		s->size *= 2;
		s->entries = (item **) realloc(s->entries, s->size * sizeof(item));
	}
	s->entries[s->top] = i;
	s->top += 1;
	return;	
}


bool empty(stack *s) // Check if the stack is empty
{
	return s->top == 0;
}
bool full(stack *s) // Check if the stack is empty
{
	return s->top == s->size;
}