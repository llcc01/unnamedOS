#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include "inc/types.h"

#define BINARY_TREE_NODE_ROOT -1
#define BINARY_TREE_NODE_LEFT 0
#define BINARY_TREE_NODE_RIGHT 1

struct binary_tree_node
{
    struct binary_tree_node *left;
    struct binary_tree_node *right;
    struct binary_tree_node *parent;
    int8_t dir;
    void *data;
};

struct binary_tree
{
    struct binary_tree_node *root;
    int (*cmp)(void *, void *);
};

struct complete_binary_tree
{
    struct binary_tree tree;
    struct binary_tree_node *rear;
};

void binary_tree_init(struct binary_tree *tree);
void binary_tree_node_swap(struct binary_tree_node *node1, struct binary_tree_node *node2);

#endif