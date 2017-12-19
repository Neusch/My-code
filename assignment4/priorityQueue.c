// This is a regular queue file given by Darrell Long, but it will be changed into
// a priority queue

# include <stdlib.h>
# include "priorityQueue.h"

queue *newQueue(uint32_t size)
{
	size++; /* need an extra blank slot */
	queue *q = (queue *) malloc(sizeof(queue));
	q->size = size;
	q->head = q->tail = 0;
	q->Q = (item **) calloc(size, sizeof(item));
	return q;
}

void delQueue(queue *q) { free(q->Q); free(q); return; }

bool emptyQ(queue *q) 
{
	return q->head == q->tail; 
}
bool fullQ(queue *q) 
{
	return (q->head + 1) % q->size == q->tail % q->size;
}

static inline uint32_t previous(uint32_t x, uint32_t n)
{
	return (x + n -1) % n;
}

static inline uint32_t next(uint32_t x, uint32_t n)
{
	return (x + n + 1) % n;
}

// Enqueue an item, it will be closer to the tail based if it is smaller than other items in the queue, the biggest items go at the head
bool enqueue(queue *q, item  *i)
{
	if (fullQ(q))
	{
		return false;
	}
	else
	{
		// This algorithm was based on ideas that Professor Long gave in class
		if (q && q->Q)
		{
			uint32_t slot = q->head;
			while (slot != q->tail && q->Q[previous(slot, q->size)]->count > i->count)
			{
				q->Q[slot] = q->Q[previous(slot, q->size)];
				slot = previous(slot, q->size);
			}
			q->Q[slot] = i;
			q->head = next(q->head, q->size);
		}
		return true;
	}
}

// Take an item from the tail
bool dequeue(queue *q, item **i)
{
	if (emptyQ(q))
	{
		return false;
	}
	else
	{
		*i = q->Q[q->tail];
		q->tail = next(q->tail,q->size);
		return true;
	}
}
