#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include "inc/types.h"

struct linked_list_node {
    struct linked_list_node *next;
    struct linked_list_node *prev;
    void *data;
};

struct linked_list {
    struct linked_list_node *head;
    struct linked_list_node *rear;
    uint32_t size;
};


struct linked_list_node *linked_list_node_create(void *data);
void linked_list_init(struct linked_list *list);
void linked_list_insert_after(struct linked_list *list, struct linked_list_node *node, void *data);
void linked_list_delete_after(struct linked_list *list, struct linked_list_node *node);



#endif