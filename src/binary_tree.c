#include <stdlib.h>
#include <stdio.h>
#include "binary_tree.h"

#define SPACES 10

void tree_draw_horizontal(ptree_node tree, int depth, char *path, int right) {
	if (tree == NULL) return;

	tree_draw_horizontal(tree->right, depth + 1, path, 1);

	for (int i = 0; i < depth; i++)
	{
		if (i == depth - 1)
			printf("+");
		else
			printf(" ");

		if (i < depth - 1) {
			for (int j = 1; j < SPACES; j++)
				printf(" ");
		}
		else {
			printf("--%.2f--", right ? tree->rvalue : tree->lvalue);
		}

		if (i == depth - 1)
			printf(">");
	}

	// Print the current node value
	printf("[%d]\n", tree->value);

	
	// go to left node
	tree_draw_horizontal(tree->left, depth + 1, path, 0);
}

//secondary function
void draw_tree_hor2(tree_node *tree, int depth, char *path, int right)
{
	// stopping condition
	if (tree == NULL) return;

	// increase spacing
	depth++;

	// start with right node
	draw_tree_hor2(tree->right, depth, path, 1);

	// set | draw map
	path[depth - 2] = 0;

	if (right)
		path[depth - 2] = 1;

	if (tree->left)
		path[depth - 1] = 1;

	// print root after spacing
	printf("\n");

	for (int i = 0; i < depth - 1; i++)
	{
		if (i == depth - 2)
			printf("+");
		else if (path[i])
			printf("|");
		else
			printf(" ");

		for (int j = 1; j < SPACES; j++)
			if (i < depth - 2)
				printf(" ");
			else
				printf("-");
	}

	printf("%d\n", tree->value);

	// vertical spacers below
	for (int i = 0; i < depth; i++)
	{
		if (path[i])
			printf("|");
		else
			printf(" ");

		for (int j = 1; j < SPACES; j++)
			printf(" ");
	}

	// go to left node
	draw_tree_hor2(tree->left, depth, path, 0);
}

//primary fuction
void draw_tree_hor(tree_node *tree)
{
	// should check if we don't exceed this somehow..
	char* path = calloc(255, 1);

	//initial depth is 0
	draw_tree_hor2(tree, 0, path, 0);
}



tree_node n1, n2, n3, n4, n5, n6, n7;
tree_node a1, a2, a3;

int mainT()
{
	srand(time(NULL));
	n1.value = 1;
	n1.lvalue = (double)rand() / RAND_MAX * 10.0;
	n1.rvalue = (double)rand() / RAND_MAX * 10.0;

	n2.value = 2;
	n2.lvalue = (double)rand() / RAND_MAX * 10.0;
	n2.rvalue = (double)rand() / RAND_MAX * 10.0;

	n3.value = 3;
	n3.lvalue = (double)rand() / RAND_MAX * 10.0;
	n3.rvalue = (double)rand() / RAND_MAX * 10.0;

	n4.value = 4;
	n4.lvalue = (double)rand() / RAND_MAX * 10.0;
	n4.rvalue = (double)rand() / RAND_MAX * 10.0;

	n5.value = 5;
	n5.lvalue = (double)rand() / RAND_MAX * 10.0;
	n5.rvalue = (double)rand() / RAND_MAX * 10.0;

	n6.value = 6;
	n6.lvalue = (double)rand() / RAND_MAX * 10.0;
	n6.rvalue = (double)rand() / RAND_MAX * 10.0;

	n7.value = 7;
	n7.lvalue = (double)rand() / RAND_MAX * 10.0;
	n7.rvalue = (double)rand() / RAND_MAX * 10.0;

	n1.right = &n2;
	n1.left = &n3;
	//n2.right = &n4;
	//n2.left = &n5;
	n3.right = &n6;
	n3.left = &n7;

	n2.right = &n3;
	n2.left = &n3;

	draw_tree_hor(&n1);

	a1.value = 1;
	a2.value = 2;
	a3.value = 3;
	a1.left = &a2;
	a1.right = &a3;

	printf("\n\n");
	tree_draw_horizontal(&n1, 0, NULL, 0);
	system("pause");
	return 0;
}