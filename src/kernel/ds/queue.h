#ifndef __QUEUE_H__
#define __QUEUE_H__

struct queue_node {
    struct queue_node *next;
    void *data;
};

struct queue {
    struct queue_node *head;
    struct queue_node *rear;
    int size;
};

void queue_init(struct queue *q);
void queue_push(struct queue *q, void *data);
void *queue_pop(struct queue *q);


#endif