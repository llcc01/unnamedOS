
#include "inc/types.h"
#include <stddef.h>

#include "ds/binary_tree.h"
#include "ds/heap.h"

void min_heap_init(min_heap_t *heap)
{
    binary_tree_init(&heap->tree);
}

void min_heap_sort_up(min_heap_t *heap, struct binary_tree_node *node)
{
    while (node->dir != BINARY_TREE_NODE_ROOT)
    {
        if (heap->tree.cmp(node->parent->data, node->data) > 0)
        {
            void *tmp = node->parent->data;
            node->parent->data = node->data;
            node->data = tmp;
        }
        node = node->parent;
    }
}

void min_heap_sort_down(min_heap_t *heap, struct binary_tree_node *node)
{
    while (node->left != NULL)
    {
        if (node->right == NULL)
        {
            if (heap->tree.cmp(node->left->data, node->data) < 0)
            {
                binary_tree_node_swap(node->left, node);
            }
            return;
        }

        struct binary_tree_node *node_op;
        if (heap->tree.cmp(node->left->data, node->right->data) < 0)
        {
            node_op = node->left;
        }
        else
        {
            node_op = node->right;
        }
        if (heap->tree.cmp(node_op->data, node->data) < 0)
        {
            binary_tree_node_swap(node_op, node);
        }
    }
}

void min_heap_insert(min_heap_t *heap, void *data)
{
    complete_binary_tree_insert(&heap->tree, data);
    min_heap_sort_up(heap, heap->rear);
}

void min_heap_get(min_heap_t *heap, void *data)
{
    if (heap->tree.root == NULL)
    {
        return;
    }
    data = heap->tree.root->data;
    binary_tree_node_swap(heap->tree.root, heap->rear);
    complete_binary_tree_delete(&heap->tree);
    min_heap_sort_down(heap, heap->tree.root);
}

void min_heap_get_and_insert(min_heap_t *heap, void *data_old, void *data_new)
{
    if (heap->tree.root == NULL)
    {
        return;
    }
    data_old = heap->tree.root->data;
    heap->tree.root->data = data_new;
    min_heap_sort_down(heap, heap->tree.root);
}