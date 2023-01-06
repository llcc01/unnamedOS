#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include "inc/types.h"

#define BINARY_TREE_NODE_LEFT 0
#define BINARY_TREE_NODE_RIGHT 1

struct binary_tree_node
{
    struct binary_tree_node *left;
    struct binary_tree_node *right;
    struct binary_tree_node *parent;
    void *data;
};

struct binary_tree
{
    struct binary_tree_node *root;
};

void binary_tree_init(struct binary_tree *tree);


#endif