#ifndef __QUEUE_HEADER
#define __QUEUE_HEADER
#include <stdbool.h>

typedef struct _Node{
    void* data; 
    struct _Node* next;
    struct _Node* prev;
} Node;

typedef struct _Queue
{
	Node* first;
	Node* last;
	int size;
	int itemSize;
} Queue;

Queue* queue_initialize(int);
bool   queue_add_at(Queue*, int, void*);
void*  queue_get_element(Queue*, int);
void*  queue_remove_element(Queue*, int);
bool   queue_enqueue(Queue*, void*);
void*  queue_dequeue(Queue*);
void*  queue_peek(Queue*);
int    queue_size(Queue*);
int    queue_index_of(Queue*, void*);
bool   queue_contains(Queue*, void*);
bool   queue_destroy(Queue*);

#endif
