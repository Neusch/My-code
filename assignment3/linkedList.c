
# include "linkedList.h"
# include "hash.h"
# include "bloomFilter.h"
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

bool moveToFront;
uint32_t seeks;

listNode *newNode(char *data,char *data2)
{
	listNode *temp;
	temp = malloc(sizeof (listNode));
	if (temp == NULL)
	{
		return NULL; // Error, malloc didnt work
	}
	else
	{
		temp->next = NULL;
		temp->prev = NULL;
		temp->oldspeak = data;
		temp->newspeak = data2;
	}	
	return temp;
}

void delNode(listNode *node)
{
	if (node == NULL)
	{
		return; // There is no list
	}
	
	if (node->next == NULL && node->prev == NULL)
	{
        // Do nothing, this is the only item in the list
    } 
	else if (node->next != NULL && node->prev != NULL)
	{
        node->next->prev = node->prev; //links the items properly
        node->prev->next = node->next; 
    } 
	else if (node->next != NULL && node->prev == NULL) 
	{
        node->next->prev = NULL;
    } 
	else if (node->next == NULL && node->prev != NULL) 
	{
        node->prev->next = NULL;
    }
	free(node->oldspeak);  // Free the memory allocated to that node
	free(node->newspeak);
	free(node);
	return;
}

void delLinkedList(listNode *list) // Assume that the head of the list is given
{
	if (list == NULL)
	{
		return; // There is no list
	}
	
	listNode *temp;
	temp = list;
	while (list->next != NULL)
	{
		temp = temp->next;
		delNode(list);
		list = temp;		
	}
	free(list); // The last item in the list needs to be freed	
}

// All new items that are inserted, are inserted at the front of the list
listNode *insertLinkedList(listNode **list, char *data,  char *data2) 
{
	listNode *prevInList;
	
	prevInList = findLinkedList(list,data); // This checks to see if the item is already in the list
	if (prevInList != NULL)
	{
		if (strcmp(prevInList->oldspeak, data) == 0)
		{
			prevInList->newspeak = data2;
			return prevInList;
		}
	}
	listNode *temp;
	if (*list == NULL) // If there are no items in the list then just create a new list
	{
		temp = newNode(data, data2);
	}
	else
	{
		temp = newNode(data, data2);
		(*list)->prev = temp;
	
		temp->prev = NULL;
		temp->next = *list;
	}
	*list = temp; // list = &temp;
	return temp;	
}

listNode *findLinkedList(listNode **list, char *data)
{
	listNode *temp; // Create a temporary pointer to modify the linked list
	listNode *head; // Create a head pointer that keeps track of the head
	temp = *list;
	head = *list;
	
	while (temp != NULL)
	{
		// strcmp = 0 when the strings are equivalent and add one to seeks for each time it goes through the linked list
		if (strcmp(temp->oldspeak, data) == 0) 
		{
			if (moveToFront == true) // Move the node to the front if it is true
			{
				if (temp->prev == NULL) // This case is for if it is already at the front
				{
					*list = head;
				}
				else if (temp->prev != NULL && temp->next != NULL) // This is the case for when there are lists on both sides
				{
					temp->next->prev = temp->prev;
					temp->prev->next = temp->next;
					temp->next = head;
					temp->prev = NULL;
					head->prev = temp; 
					*list = temp;
				}
				else if (temp->next == NULL && temp->prev != NULL)
				{
					temp->next = head;
					temp->prev->next = NULL;
					temp->prev = NULL;
					*list = temp;
				}
				//*list = temp; // Set the front of the list to the new front
			}
			else
			{
				*list = head; // Set the list back to the head
			}
			return temp; // Return the node where the item was found
		}
		if (temp->next != NULL)
		{
			seeks++;
			temp = temp->next;
		}
		else
		{
			seeks++;
			break;
		}
	}
	return NULL; // If the string could not be found then return NULL
}
// This function assumes that the item in the list is the first one
void printLinkedList(listNode *head) 
{
	if (head == NULL)
	{
		return; // There is nothing in the list
	}
	listNode *temp;
	temp = head;
	
	while (temp != NULL)
	{
		if (strcmp(temp->oldspeak,"Start") == 0) // If there is nothing at that point in the list, then just continue
		{
			break;
		}
		else if (temp->newspeak != NULL)
		{
			printf("\n%s -> %s", temp->oldspeak,temp->newspeak); // Print out each string being held
        }
		else
		{
			printf("\n%s",temp->oldspeak);
		}		
		
		if (temp->next != NULL)
		{
			temp = temp->next;		
		}
		else
		{
			break;
		}
	}
	printf("\n");
	return;	
	
	// This is used when I want to test printing out individual nodes, I kept it because I thought it could be useful later
	/*
	if (head == NULL)
	{
		return; // There is nothing in the list
	}
	if (head->newspeak == NULL)
	{
		printf("\n%s -> NULL", head->oldspeak);//Print out each string being held
	}
	else
	{
		printf("\n%s -> %s", head->oldspeak,head->newspeak);//Print out each string being held
	}
	return;*/
}
