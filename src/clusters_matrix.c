#include "clusters_matrix.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <float.h>

/* Inline sections */

int inline matrix_to_array(int row, int col, int n) {
	return (row * n) + col;
}

int inline row_to_cluster(int row) {
	return row + 1;
}

/* Private section */

/**
* Calculates the distances between each cluster for the current instance
* Distances are stored in the first usable row and in the first usable column of the matrix
* Since the clustering matrix is symmetric, we can use a column/row to store distances
* and so avoid a zeroed row/col
**/
static void clusters_calculate_distances(pclusters_matrix instance) {
	// Current clusters count. We can use this as row/column index in the matrix
	// in orther to store the distances
	int clusters = instance->current_cluster_count;
	// Original clusters count
	int size = instance->initial_cluster_count;
	int* matrix = instance->matrix;

	for (int i = 0; i < clusters - 1; i++)
	{
		/* in this case, i represents a cluster with index [B .. Z],
		* that correspond to a row in our matrix.
		* j is only used to traverse the row
		*/
		for (int j = 0; j <= i; j++)
		{
			matrix[matrix_to_array(i, clusters - 1, size)] += matrix[matrix_to_array(i, j, size)];
		}

		/* in this case, i represents a cluster with index [A .. Y],
		* that correspond to a col in our matrix.
		* j is only used to traverse the col from below the diagonal
		*/
		for (int j = i; j < clusters - 1; j++)
		{
			matrix[matrix_to_array(clusters - 1, i, size)] += matrix[matrix_to_array(j, i, size)];
		}
	}
}

static int clusters_cumulative_distance(pclusters_matrix instance, int cluster) {
	int dist = 0;
	int clusters = instance->current_cluster_count;
	// If we are looking for the distance for the clusters [A - Y], we search the value in the sum row
	if (cluster <= clusters - 1) dist += instance->matrix[matrix_to_array(clusters - 1, cluster, instance->initial_cluster_count)];

	// If we are looking for the distance for the clusters [B - Z], we search the value in the sum col
	// The cluster index must be decremented since we use a "reduced" simmetric matrix
	if (cluster > 0) dist += instance->matrix[matrix_to_array(cluster - 1, clusters - 1, instance->initial_cluster_count)];

	return dist;
}


static double clusters_separation_degree(pclusters_matrix instance, int colCluster, int rowCluster) {
	// We have to compute degree = D(C1, C2) - U(C1) -U(C2)
	// Since we use half of the simmetric matrix, cluster2 must represent a 
	int colClusterC = clusters_cumulative_distance(instance, colCluster);
	int rowClusterC = clusters_cumulative_distance(instance, rowCluster + 1);

	double degree = (double)instance->matrix[matrix_to_array(rowCluster, colCluster, instance->initial_cluster_count)];
	degree -= (colClusterC + rowClusterC) / (instance->current_cluster_count - 2);
	return degree;
}

static void clusters_search_closest(pclusters_matrix instance, int* cluster1, int* cluster2, double* best_separation_degree) {
	*cluster1 = -1;
	*cluster2 = -1;
	*best_separation_degree = DBL_MAX;

	int clusters = instance->current_cluster_count;
	int size = instance->initial_cluster_count;
	int* matrix = instance->matrix;

	// i represents clusters from A[0] to Y [cols in the matrix]
	for (int i = 0; i < clusters - 1; i++)
	{
		// j represents clusters from B[0] to Z
		for (int j = i; j < clusters - 1; j++)
		{
			double separation_degree = clusters_separation_degree(instance, i, j);
			//printf("Degree for C%d-C%d: %f\n", i, j + 1, separation_degree);
			if (separation_degree < *best_separation_degree)
			{
				*best_separation_degree = separation_degree;
				*cluster1 = i;
				*cluster2 = j;
			}
		}
	}
}

static void clusters_update_distances(pclusters_matrix instance, int col, int row, int oldDistance) {
	int clusters = instance->current_cluster_count;
	int size = instance->initial_cluster_count;
	int* matrix = instance->matrix;
	// We have to update all the distances for the new col element and new row element
	for (int i = row; i < clusters - 1; i++)
	{
		matrix[matrix_to_array(i, col, size)] = (matrix[matrix_to_array(i + 1, col, size)] + matrix[matrix_to_array(i + 1, col + 1, size)] - oldDistance) / 2;
	}

	for (int i = 0; i < col; i++)
	{
		matrix[matrix_to_array(row - 1, i, size)] = (matrix[matrix_to_array(row - 1, i, size)] + matrix[matrix_to_array(row, i, size)] - oldDistance) / 2;
	}

	// We have to move backward the columns since we have "lost" a cluster 
	for (int i = row + 1; i < clusters; i++)
	{
		for (int j = 0; j < col; j++)
		{
			matrix[matrix_to_array(i - 1, j, size)] = matrix[matrix_to_array(i, j, size)];
		}

		for (int j = col + 2; j < clusters; j++)
		{
			matrix[matrix_to_array(i - 1, j - 1, size)] = matrix[matrix_to_array(i, j, size)];
		}
	}

	for (int i = 0; i < clusters - 1; i++)
	{
		matrix[matrix_to_array(i, clusters - 1, size)] = 0;
		matrix[matrix_to_array(clusters - 1, i, size)] = 0;
	}
	clusters_calculate_distances(instance);
}

/* Public section */
pclusters_matrix clusters_create_matrix(int * source, int size)
{
	size_t matrix_type_size = sizeof(clusters_matrix);
	pclusters_matrix new_matrix = (pclusters_matrix)malloc(matrix_type_size);

	new_matrix->initial_cluster_count = size;
	new_matrix->current_cluster_count = size;
	new_matrix->matrix = (int*)calloc(size * size, sizeof(int));

	for (int i = 1; i < size; i++)
	{
		for (int j = 0; j < size - 1; j++)
		{
			new_matrix->matrix[matrix_to_array(i - 1, j, size)] = source[matrix_to_array(i, j, size)];
		}
	}
	clusters_calculate_distances(new_matrix);
	return new_matrix;
}

void clusters_free_distances(pclusters_matrix instance)
{
	if (instance == 0) return;

	free(instance->matrix);
	instance->matrix = 0;
	instance->initial_cluster_count = 0;
	instance->current_cluster_count = 0;
}

int clusters_increase_clustering(pclusters_matrix instance) {
	if (instance == 0) return 0;
	if (instance->current_cluster_count == 1) return 1;

	int c1;
	int c2;
	double best;
	clusters_search_closest(instance, &c1, &c2, &best);

	// TODO -> Store data for tree
	instance->current_cluster_count--;
	clusters_update_distances(instance, c1, c2, instance->matrix[matrix_to_array(c2, c1, instance->initial_cluster_count)]);
}

void clusters_print_matrix(pclusters_matrix matrix)
{
	if (matrix == 0) return;

	int clusters = matrix->current_cluster_count;
	int size = matrix->initial_cluster_count;

	// Print table header
	printf("%10s|", "Clusters");
	for (size_t i = 0; i < clusters - 1; i++)
	{
		printf("C%-4d|", i);

	}
	printf("%-5s|", "SUM");
	printf("\n");

	// Print table rows
	for (int i = 0; i < clusters - 1; i++)
	{
		printf("%9s%d|", "C", row_to_cluster(i));
		for (int j = 0; j < clusters; j++)
		{
			printf("%5d|", matrix->matrix[matrix_to_array(i, j, size)]);
		}
		printf("\n");
	}

	// print last sum row
	printf("%10s|", "SUM");
	for (int j = 0; j < clusters - 1; j++)
	{
		printf("%5d|", matrix->matrix[matrix_to_array(clusters - 1, j, size)]);
	}
	printf("\n");
}
