#ifndef _BINARY_TREE_H
#define _BINARY_TREE_H

typedef struct tree_node tree_node, *ptree_node;

typedef struct tree_node {
	int value; ///< Value of the node
	ptree_node left; ///< Left node
	double lvalue; ///< Value of the node
	ptree_node right; ///< Right node
	double rvalue; ///< Value of the node

};
#endif // !_BINARY_TREE_H
