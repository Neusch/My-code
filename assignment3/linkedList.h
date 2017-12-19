

# ifndef ll_h
# define ll_h

# include <stdbool.h>
# include <stdint.h>

extern bool moveToFront;
// This is used to track stats, but also needs to be accessed in main to be printed, so I declare it extern
extern uint32_t seeks; 

typedef struct listNode listNode;

struct listNode {
	char *oldspeak;
	char *newspeak;
	listNode *prev;
	listNode *next;
};

listNode *newNode( char *data, char *data2);

void delNode(listNode *item);

void delLinkedList(listNode *item);

listNode *insertLinkedList(listNode **list,char *data,char *data2);

listNode *findLinkedList(listNode **list, char *data);

void printLinkedList(listNode *head);



# endif // ll_h