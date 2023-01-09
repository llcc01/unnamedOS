#include "utils/malloc.h"
#include "utils/printf.h"
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
    node->dir = BINARY_TREE_NODE_ROOT;
    node->data = data;
    return node;
}

void binary_tree_node_destroy(struct binary_tree_node *node)
{
    free(node);
}

inline void binary_tree_node_swap(struct binary_tree_node *node1, struct binary_tree_node *node2)
{
    void *tmp = node1->data;
    node1->data = node2->data;
    node2->data = tmp;
}

void binary_tree_init(struct binary_tree *tree)
{
    tree->root = NULL;
}

void binary_tree_node_dump(struct binary_tree_node *node, uint16_t level)
{
    for (int i = 0; i < level; i++)
    {
        printf("  ");
    }
    if (node == NULL)
    {
        printf("-\n");
        return;
    }
    struct binary_tree_node *l = node->left;
    struct binary_tree_node *r = node->right;

    printf("%p %p\n", node, node->data);
    binary_tree_node_dump(l, level + 1);
    binary_tree_node_dump(r, level + 1);
}

void binary_tree_dump(struct binary_tree *tree)
{
    binary_tree_node_dump(tree->root, 0);
}

void complete_binary_tree_insert(struct complete_binary_tree *cbt, void *data)
{
    struct binary_tree *tree = &cbt->tree;
    struct binary_tree_node *node_new = binary_tree_node_creat(data);
    if (node_new == NULL)
    {
        return;
    }
    // printf("complete_binary_tree_insert %p %p\n", node_new, node_new->data);
    if (tree->root == NULL)
    {
        tree->root = node_new;
        cbt->rear = node_new;
        return;
    }

    if (cbt->rear->dir == BINARY_TREE_NODE_LEFT)
    {
        cbt->rear->parent->right = node_new;
        node_new->parent = cbt->rear->parent;
        node_new->dir = BINARY_TREE_NODE_RIGHT;
        cbt->rear = node_new;
        return;
    }

    struct binary_tree_node *node = cbt->rear;
    while (node->dir == BINARY_TREE_NODE_RIGHT)
    {
        node = node->parent;
    }

    if (node->dir != BINARY_TREE_NODE_ROOT)
    {
        node = node->parent->right;
    }

    while (node->left != NULL)
    {
        node = node->left;
    }

    node->left = node_new;
    node_new->parent = node;
    node_new->dir = BINARY_TREE_NODE_LEFT;
    cbt->rear = node_new;
}

void complete_binary_tree_del_rear(struct complete_binary_tree *cbt)
{
    struct binary_tree *tree = &cbt->tree;
    if (tree->root == NULL)
    {
        return;
    }
    if (cbt->rear->dir == BINARY_TREE_NODE_RIGHT)
    {
        struct binary_tree_node *parent = cbt->rear->parent;
        cbt->rear = parent->left;
        // mark();
        binary_tree_node_destroy(parent->right);
        parent->right = NULL;
        return;
    }

    struct binary_tree_node *node = cbt->rear;
    while (node->dir == BINARY_TREE_NODE_LEFT)
    {
        node = node->parent;
    }

    if (node->dir != BINARY_TREE_NODE_ROOT)
    {
        node = node->parent->left;
    }

    while (node->right != NULL)
    {
        node = node->right;
    }

    cbt->rear->parent->left = NULL;
    // mark();
    binary_tree_node_destroy(cbt->rear);
    cbt->rear = node;
}