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
    void *data;
    int8_t dir;
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

struct binary_tree_node *binary_tree_node_creat(void *data);
void binary_tree_node_destroy(struct binary_tree_node *node);
void binary_tree_node_swap(struct binary_tree_node *node1, struct binary_tree_node *node2);


void binary_tree_init(struct binary_tree *tree);
void binary_tree_dump(struct binary_tree *tree);


void complete_binary_tree_insert(struct complete_binary_tree *cbt, void *data);
void complete_binary_tree_del_rear(struct complete_binary_tree *cbt);


#endif