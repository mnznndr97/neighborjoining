#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "binary_tree.h"

#define SPACES 10

/**
 * Frees the resources associated to a binary tree with a recoursive depth-first visit
 *
 * @param node Root of a binary tree
 */
static void btree_free_df_r(pbtree_node node) {
    if (!node) return;

    btree_free_df_r(node->left);
    btree_free_df_r(node->right);

    free(node->name);
    memset(node, 0, sizeof(btree_node));
    free(node);
}

/**
 * Return the depth of a binary tree
 *
 * @param node Root of a binary tree
 * @returns Depth of a binary tree
 */
static size_t tree_get_depth(const btree_node * tree) {
    if (!tree) return 0;

    size_t left_depth = tree_get_depth(tree->left);
    size_t right_depth = tree_get_depth(tree->right);

    return 1 + ((left_depth > right_depth) ? left_depth : right_depth);
}

/**
 * Print recursively the binary tree to the console
 * @param tree Node that represent the oot of the tree at the current level
 * @param parent Parent of the current node
 * @param depth Level of depth for the current node.
 * @param right Indicates if we are in a right branch
 * @param levels Helper array
 */
static void
btree_draw_horizontal_r(const btree_node * tree, const btree_node * parent, const int depth, const int right,
                        char *levels) {
    if (tree == NULL) return;

    // Let's go with the right node first
    btree_draw_horizontal_r(tree->right, tree, depth + 1, 1, levels);

    if (depth > 0) {
        for (int i = 1; i < depth; i++) {
            // starting from level 1,
            printf(levels[i] ? "|" : " ");
            for (int j = 1; j < SPACES; j++)
                printf(" ");
        }
        // if we are printing a node that is not the root of the three,
        // we need to also print the edges with the associated value
        printf("+--%.2f-->", right ? parent->rvalue : parent->lvalue);
    }
    // Print the current node name
    printf("[%s]\n", tree->name);

    // levels keeps track of the direction of the branch that we have traversed at a certain depth
    // levels[i] is true if we have taken the left branch at level i to get to the current node
    levels[depth] = !levels[depth];
    // Let's go to left node
    btree_draw_horizontal_r(tree->left, tree, depth + 1, 0, levels);
}

/**
 * Print a binary tree horizontally on the console
 *
 * @param tree Root of a binary tree
 */
void btree_draw_horizontal(const btree_node *tree) {
    if (!tree) return;

    // Let's use a support array in order to print the edges for the tree
    char *levels = calloc(tree_get_depth(tree), sizeof(char));
    btree_draw_horizontal_r(tree, NULL, 0, 0, levels);
    free(levels);
}

/**
 * Frees the resources associated to a binary tree
 *
 * @param node Root of a binary tree
 */
void btree_free(pbtree_node tree) {
    btree_free_df_r(tree);
}