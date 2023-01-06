#ifndef __HEAP_H__
#define __HEAP_H__

#include "ds/binary_tree.h"

typedef struct complete_binary_tree min_heap_t;

void min_heap_init(min_heap_t *heap);
void min_heap_sort_up(min_heap_t *heap, struct binary_tree_node *node);
void min_heap_sort_down(min_heap_t *heap, struct binary_tree_node *node);
void min_heap_insert(min_heap_t *heap, void *data);
void *min_heap_get(min_heap_t *heap);
void *min_heap_get_and_insert(min_heap_t *heap, void *data_ins);

void *min_heap_get_top_value(min_heap_t *heap);
void min_heap_set_top_value(min_heap_t *heap , void *data);


#endif