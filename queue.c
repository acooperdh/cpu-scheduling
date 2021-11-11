#include "queue.h"
#include <stdlib.h>

Queue* queue_initialize(int typeSize)
{
	Queue* queue = malloc(sizeof(*queue));

	if(queue == NULL)
	{
		return NULL;
	}

    queue->first = NULL;
    queue->last = NULL;
    queue->size = 0;
    queue->itemSize = typeSize;

	return queue;	
}

bool queue_add_at(Queue* queue, int index, void* element){
    if (queue == NULL || element == NULL)
        return false;

    if(index < 0 || index > queue->size)
        return false;

    Node* newNode = malloc(sizeof(*newNode));
    newNode->data = malloc(queue->itemSize);
    memcpy(newNode->data, element, queue->itemSize);

    if (queue->size == 0){
        queue->first = newNode;
        queue->last = newNode;
        newNode->next = NULL;
        newNode->prev = NULL;
    }
    else if (index == 0){
        newNode->next = queue->first;
        newNode->prev = NULL;
        queue->first->prev = newNode;
        queue->first = newNode;
    }
    else if (index == queue->size){
        newNode->next = NULL;
        newNode->prev = queue->last;
        queue->last->next = newNode;
        queue->last = newNode;
    }
    else{
        Node* current = queue->first;
        for (int i = 0; i < index; i++)
            current = current->next;
        newNode->prev = current->prev;
        newNode->next = current;
        current->prev->next = newNode;
        current->prev = newNode;
    }

    queue->size++;
}

void* queue_get_element(Queue* queue, int index){
    if (queue == NULL)
        return NULL;

    if(index < 0 || index >= queue->size)
        return NULL;

    Node* current = queue->first;
    for (int i = 0; i < index; i++)
        current = current->next;

    return current->data;
}

void* queue_remove_element(Queue* queue, int index){
    if (queue == NULL)
        return NULL;

    if(index < 0 || index >= queue->size)
        return NULL;

    Node* current = queue->first;
    for (int i = 0; i < index; i++)
        current = current->next;
    
    if (queue->size == 1){
        queue->first = NULL;
        queue->last = NULL;
    }
    else if (index == 0){
        queue->first = current->next;
        queue->first->prev = NULL;
    }
    else if(index == queue->size-1){
        queue->last = current->prev;
        queue->last->next = NULL;
    }
    else{
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }

    queue->size--;
    
    void* data = current->data;

    free(current);

    return data;
}

bool queue_enqueue(Queue* queue, void* element)
{
	if(queue == NULL)
		return false;
	return queue_add_at(queue, queue->size, element);
}

void* queue_dequeue(Queue* queue)
{
	if(queue == NULL)
		return NULL;
	
	void* data = queue_remove_element(queue, 0);

	return data;
}

void* queue_peek(Queue* queue)
{
	return queue_get_element(queue, 0);
}

int queue_size(Queue* queue)
{
	if(queue == NULL)
		return -1;
	
	return queue->size;
}

int queue_index_of(Queue* queue, void* element){
    if (queue == NULL || element == NULL)
        return -1;

    Node* current = queue->first;
    for (int i = 0; i < queue->size; i++)
    {
        if (memcmp(current->data, element, queue->itemSize) == 0)
            return i;
        current = current->next;
    }

    return -1;
}

bool queue_contains(Queue* queue, void* element)
{
	if(queue == NULL)
		return NULL;
	
	int index = queue_index_of(queue, element);

	return (index >= 0 ? true : false);
}

bool queue_destroy(Queue* queue)
{
	if(queue == NULL)
		return false;

	if(queue->size > 0){
        Node* current = queue->first;
        Node* next = NULL;

        while(current != NULL){
            next = current->next;
            free(current->data);
            free(current);
            current = next;
        }
    }

	free(queue);

	return true;
}

Queue* queue_copy(Queue* queue) {
    if (queue == NULL)
        return NULL;

    Queue* newQueue = queue_initialize(queue->itemSize);

    if (newQueue == NULL)
        return NULL;

    Node* current = queue->first;
    while (current != NULL)
    {
        queue_add_at(newQueue, newQueue->size, current->data);
        current = current->next;
    }

    return newQueue;
}