#ifndef _BINARY_TREE_H
#define _BINARY_TREE_H

typedef struct _btree_node btree_node, *pbtree_node;

typedef struct _btree_node {
    char *name; ///< Value of the node
    pbtree_node left; ///< Left node
    double lvalue; ///< Value of the edge to the left node
    pbtree_node right; ///< Right node
    double rvalue; ///< Value of the edge to the right node

} btree_node, *pbtree_node;

void btree_draw_horizontal(const btree_node *tree);
void btree_free(pbtree_node tree);

#endif // !_BINARY_TREE_H
