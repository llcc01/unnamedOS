


#include "inc/types.h"
#include <stddef.h>

#include "utils/malloc.h"

#include "ds/linked_list.h"


struct linked_list_node *linked_list_node_create(void *data)
{
    struct linked_list_node *node = (struct linked_list_node *)malloc(sizeof(struct linked_list_node));
    if(node == NULL)
        return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}

void linked_list_init(struct linked_list *list)
{
    list->head = linked_list_node_create(NULL);
    list->rear = list->head;
    list->size = 0;
}

void linked_list_insert_after(struct linked_list *list, struct linked_list_node *node, void *data)
{
    struct linked_list_node *node_new = linked_list_node_create(data);
    node_new->next = node->next;
    node->next = node_new;
    list->size++;
}

void linked_list_delete_after(struct linked_list *list, struct linked_list_node *node)
{
    struct linked_list_node *node_delete = node->next;
    node->next = node_delete->next;
    free(node_delete);
    list->size--;
}