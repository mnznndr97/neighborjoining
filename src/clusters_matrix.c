#include "clusters_matrix.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <float.h>

/* Inline sections */

int inline clusters_to_index(int cluster1, int cluster2) {
	return (cluster2 * (cluster2 - 1) / 2) + cluster1;
}

int inline clusters_cumulative_distance(pclusters_matrix instance, int cluster) {
	return instance->matrix[instance->matrix_size + cluster];
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

	// For each cluster we need to calculate the cumulative distance
	for (int i = 0; i < clusters; i++)
	{
		for (int j = 0; j < i; j++)
		{
			matrix[instance->matrix_size + i] += matrix[clusters_to_index(j, i)];
			printf("[DBG] Cluster (%d)[%d] += %d(%d, %d)[%d]\n", i, instance->matrix_size + i, matrix[clusters_to_index(j, i)], j, i, clusters_to_index(j, i));
		}

		for (int j = i + 1; j < clusters; j++)
		{
			matrix[instance->matrix_size + i] += matrix[clusters_to_index(i, j)];
			printf("[DBG] Cluster (%d)[%d] += %d(%d, %d)[%d]\n", i, instance->matrix_size + i, matrix[clusters_to_index(i, j)], i, j, clusters_to_index(i, j));
		}
	}
}

static double clusters_separation_degree(pclusters_matrix instance, int fCluster, int sCluster) {
	// We have to compute degree = D(C1, C2) - U(C1) -U(C2)
	// Since we use half of the simmetric matrix, cluster2 must represent a 
	int fClusterC = clusters_cumulative_distance(instance, fCluster);
	int sClusterC = clusters_cumulative_distance(instance, sCluster);

	double degree = (double)instance->matrix[clusters_to_index(fCluster, sCluster)];
	degree -= (fClusterC + sClusterC) / (instance->current_cluster_count - 2);
	return degree;
}

static void clusters_search_closest(pclusters_matrix instance, int* cluster1, int* cluster2, double* best_separation_degree) {
	*cluster1 = -1;
	*cluster2 = -1;
	*best_separation_degree = DBL_MAX;

	int clusters = instance->current_cluster_count;
	int size = instance->initial_cluster_count;
	int* matrix = instance->matrix;

	for (int i = 0; i < clusters - 1; i++)
	{
		for (int j = i + 1; j < clusters; j++)
		{
			double separation_degree = clusters_separation_degree(instance, i, j);
			printf("Degree for C%d-C%d: %f\n", i, j, separation_degree);
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
		matrix[clusters_to_index(i, col)] = (matrix[clusters_to_index(i + 1, col)] + matrix[clusters_to_index(i + 1, col + 1)] - oldDistance) / 2;
	}

	for (int i = 0; i < col; i++)
	{
		matrix[clusters_to_index(row - 1, i)] = (matrix[clusters_to_index(row - 1, i)] + matrix[clusters_to_index(row, i)] - oldDistance) / 2;
	}

	// We have to move backward the columns since we have "lost" a cluster 
	for (int i = row + 1; i < clusters; i++)
	{
		for (int j = 0; j < col; j++)
		{
			matrix[clusters_to_index(i - 1, j)] = matrix[clusters_to_index(i, j)];
		}

		for (int j = col + 2; j < clusters; j++)
		{
			matrix[clusters_to_index(i - 1, j - 1)] = matrix[clusters_to_index(i, j)];
		}
	}

	for (int i = 0; i < clusters; i++)
	{
		matrix[instance->matrix_size + i] = 0;
	}
	clusters_calculate_distances(instance);
}

/* Public section */
pclusters_matrix clusters_create_matrix(int* source, int size)
{
	size_t matrix_type_size = sizeof(clusters_matrix);
	pclusters_matrix new_instance = (pclusters_matrix)calloc(1, matrix_type_size);

	if (new_instance) {
		// Allocation for the triangular matrix with all zeros
		int matrix_size = (size * (size - 1) / 2) + size;
		new_instance->matrix = (int*)calloc(matrix_size, sizeof(int));

		if (new_instance->matrix) {
			new_instance->initial_cluster_count = size;
			new_instance->current_cluster_count = size;
			new_instance->matrix_size = matrix_size - size;
			// Fill out matrix from the source distances matrix
			for (int i = 0; i < size - 1; i++)
			{
				for (int j = i + 1; j < size; j++)
				{
					new_instance->matrix[clusters_to_index(i, j)] = source[i + j * size];
				}
			}
			clusters_calculate_distances(new_instance);
		}
		else
		{
			// In case of error
			clusters_free_distances(new_instance);
			new_instance = 0;
		}
	}
	return new_instance;
}

void clusters_free_distances(pclusters_matrix instance)
{
	if (!instance) return;

	if (instance->matrix)
		free(instance->matrix);
	instance->matrix = 0;
	instance->initial_cluster_count = 0;
	instance->current_cluster_count = 0;

	free(instance);
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
	instance->matrix_size -= instance->current_cluster_count;
	clusters_update_distances(instance, c1, c2, instance->matrix[clusters_to_index(c1, c2)]);
}

void clusters_print_matrix(pclusters_matrix matrix)
{
	if (!matrix) return;

	int clusters = matrix->current_cluster_count;
	int size = matrix->initial_cluster_count;

	// Print table header
	printf("%10s|", "Clusters");
	for (int i = 0; i < clusters; i++)
	{
		printf("C%-4d|", i);

	}
	printf("\n");

	// Print table rows
	for (int i = 1; i < clusters; i++)
	{
		printf("%9s%d|", "C", i);
		for (int j = 0; j < i; j++)
		{
			printf("%5d|", matrix->matrix[clusters_to_index(j, i)]);
		}
		printf("\n");
	}

	// print last sum row
	printf("%10s|", "SUM");
	for (int j = 0; j < clusters; j++)
	{
		printf("%5d|", matrix->matrix[matrix->matrix_size + j]);
	}
	printf("\n");
}
