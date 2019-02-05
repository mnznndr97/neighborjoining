#include <stdlib.h>
#include <stdio.h>
#include "binary_tree.h"

#define SPACES 10

static void tree_draw_horizontal_r(ptree_node tree, ptree_node parent, int depth, int right) {
    if (tree == NULL) return;

    tree_draw_horizontal_r(tree->right, tree, depth + 1, 1);

    for (int i = 0; i < depth; i++) {
        if (i == depth - 1)
            printf("+");
        else
            printf(" ");

        if (i < depth - 1) {
            for (int j = 1; j < SPACES; j++)
                printf(" ");
        } else {
            printf("--%.2f--", right ? parent->rvalue : parent->lvalue);
        }

        if (i == depth - 1)
            printf(">");
    }

    // Print the current node value
    printf("[%s]\n", tree->value);


    // go to left node
    tree_draw_horizontal_r(tree->left, tree, depth + 1, 0);
}

void tree_draw_horizontal(ptree_node tree) {
    tree_draw_horizontal_r(tree, NULL, 0, 0);
}