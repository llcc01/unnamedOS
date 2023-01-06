

#include "inc/types.h"
#include <stddef.h>

#include "utils/malloc.h"
#include "ds/queue.h"

void queue_init(struct queue *q)
{
    struct queue_node *node = (struct queue_node *)malloc(sizeof(struct queue_node));
    node->data = NULL;
    node->next = NULL;
    q->head = node;
    q->rear = node;
    q->size = 0;
}

void queue_push(struct queue *q, void *data)
{
    struct queue_node *node = (struct queue_node *)malloc(sizeof(struct queue_node));
    node->data = data;
    node->next = NULL;
    q->rear->next = node;
    q->rear = node;
    q->size++;
}

void *queue_pop(struct queue *q)
{
    if (q->size == 0)
        return NULL;
    struct queue_node *node = q->head->next;
    q->head = node->next;
    void *data = node->data;
    free(node);
    q->size--;
    return data;
}