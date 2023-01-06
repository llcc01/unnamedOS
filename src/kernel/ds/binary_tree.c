#include "utils/malloc.h"
#include "ds/binary_tree.h"

struct binary_tree_node *binary_tree_node_creat(void *data)
{
    struct binary_tree_node *node = (struct binary_tree_node *)malloc(sizeof(struct binary_tree_node));
    if (node == NULL)
    {
        return NULL;
    }
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->data = data;
}

void binary_tree_init(struct binary_tree *tree)
{
    tree->root = NULL;
}

