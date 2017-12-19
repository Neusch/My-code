# ifndef _QUEUE_H
# define _QUEUE_H

// A queue header file provided by Darrell Long

# include "huffman.h"
# include <stdint.h>
# include <stdbool.h>
# include "stack.h"

//typedef uint32_t item;//treeNode item;

typedef struct queue 
{
	uint32_t size; // How big is the queue
	uint32_t head, tail; // Track the front and back of the queue
	item **Q; // An array of items
} queue;

queue *newQueue(uint32_t size); // Construct a new queue

void delQueue(queue *q); // Destructor

bool emptyQ(queue *q);

bool fullQ(queue *q);

bool enqueue(queue *q, item *i);

bool dequeue(queue *q, item **i);

# endif
