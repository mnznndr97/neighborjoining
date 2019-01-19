#include<stdio.h>
#include<stdlib.h>
#include "clusters_matrix.h"

int main(int argc, char* argv[]) {

	int* source_matrix = (int*)calloc(6 * 6, sizeof(int));

	source_matrix[0] = 0;
	source_matrix[6] = 5; source_matrix[7] = 0;
	source_matrix[12] = 4; source_matrix[13] = 7; source_matrix[14] = 0;
	source_matrix[18] = 7; source_matrix[19] = 10; source_matrix[20] = 7; source_matrix[21] = 0;
	source_matrix[24] = 6; source_matrix[25] = 9; source_matrix[26] = 6; source_matrix[27] = 5; source_matrix[28] = 0;
	source_matrix[30] = 8; source_matrix[31] = 11; source_matrix[32] = 8; source_matrix[33] = 8; source_matrix[34] = 8; source_matrix[35] = 0;

	pclusters_matrix matrix = clusters_create_matrix(source_matrix, 6);
	clusters_print_matrix(matrix);

	clusters_increase_clustering(matrix);
	clusters_print_matrix(matrix);

	clusters_increase_clustering(matrix);
	clusters_print_matrix(matrix);

	clusters_increase_clustering(matrix);
	clusters_print_matrix(matrix);


	clusters_increase_clustering(matrix);
	clusters_print_matrix(matrix);


	clusters_free_distances(matrix);

	system("pause");
}

void neighbor_joining(int** matrix, int n) {


}