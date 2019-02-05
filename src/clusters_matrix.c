#include "clusters_matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <assert.h>
#include <string.h>

/* Inline sections */

static inline int clusters_to_index(int cluster1, int cluster2) {
    assert(cluster1 < cluster2);
    return (cluster2 * (cluster2 - 1) / 2) + cluster1;
}

static inline double clusters_cumulative_distance(pclusters_matrix instance, int cluster) {
    return instance->matrix[instance->matrix_size + cluster];
}


/* Private section */

static ptree_node clusters_create_node(pclusters_matrix instance, int cluster){
    ptree_node node = calloc(1, sizeof(tree_node));
    char *newStr = calloc(strlen(instance->clusters_names[cluster]) + 1, sizeof(char));
    strcpy(newStr, instance->clusters_names[cluster]);
    node->value = newStr;

    return node;
}

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
    double *matrix = instance->matrix;

    // For each cluster we need to calculate the cumulative distance
    for (int i = 0; i < clusters; i++) {
        for (int j = 0; j < i; j++) {
            matrix[instance->matrix_size + i] += matrix[clusters_to_index(j, i)];
        }

        for (int j = i + 1; j < clusters; j++) {
            matrix[instance->matrix_size + i] += matrix[clusters_to_index(i, j)];
        }
    }
}

static double clusters_separation_degree(pclusters_matrix instance, int fCluster, int sCluster) {
    // We have to compute degree = D(C1, C2) - U(C1) -U(C2)
    // Since we use half of the simmetric matrix, cluster2 must represent a
    double fClusterC = clusters_cumulative_distance(instance, fCluster);
    double sClusterC = clusters_cumulative_distance(instance, sCluster);

    double degree = instance->matrix[clusters_to_index(fCluster, sCluster)];
    degree -= (fClusterC + sClusterC) / (instance->current_cluster_count - 2);
    return degree;
}

static void
clusters_search_closest(pclusters_matrix instance, int *cluster1, int *cluster2, double *best_separation_degree) {
    *cluster1 = -1;
    *cluster2 = -1;
    *best_separation_degree = DBL_MAX;

    int clusters = instance->current_cluster_count;

    for (int i = 0; i < clusters - 1; i++) {
        for (int j = i + 1; j < clusters; j++) {
            double separation_degree = clusters_separation_degree(instance, i, j);
#ifndef DEBUG
            printf("Degree for %s-%s: %f\n", instance->clusters_names[i], instance->clusters_names[j], separation_degree);
#endif
            if (separation_degree < *best_separation_degree) {
                *best_separation_degree = separation_degree;
                *cluster1 = i;
                *cluster2 = j;
            }
        }
    }
}

static void clusters_update_distances(pclusters_matrix instance, int c1, int c2, double oldDistance) {
    int clusters = instance->current_cluster_count;
    int size = instance->initial_cluster_count;
    double *matrix = instance->matrix;
    assert(c1 < c2);

    // We have to update all the distances for the new c1 element and new c2 element
    for (int i = c1 + 1; i < clusters; i++) {
        if (i == c2) continue;
        else if (i < c2)
            matrix[clusters_to_index(c1, i)] += matrix[clusters_to_index(i, c2)];
        else
            matrix[clusters_to_index(c1, i)] += matrix[clusters_to_index(c2, i)];
        matrix[clusters_to_index(c1, i)] = (matrix[clusters_to_index(c1, i)] - oldDistance) / 2;
    }

    for (int i = 0; i < c1; i++) {
        matrix[clusters_to_index(i, c1)] =
                (matrix[clusters_to_index(i, c1)] + matrix[clusters_to_index(i, c2)] - oldDistance) / 2;
    }

    // We have to move backward the columns since we have "lost" a cluster
    for (int i = c2 + 1; i < clusters; i++) {
        for (int j = 0; j < c2; j++) {
            matrix[clusters_to_index(j, i - 1)] = matrix[clusters_to_index(j, i)];
        }

        for (int j = c2 + 1; j < clusters; j++) {
            if (i == j) continue;
            else if (j < i)
                matrix[clusters_to_index(j - 1, i - 1)] = matrix[clusters_to_index(j, i)];
            else
                matrix[clusters_to_index(i - 1, j - 1)] = matrix[clusters_to_index(i, j)];
        }
    }

    for (int i = 0; i < clusters; i++) {
        matrix[instance->matrix_size + i] = 0;
    }
    instance->current_cluster_count--;
    int required = snprintf(NULL, 0, "J%d", instance->initial_cluster_count - instance->current_cluster_count);
    free(instance->clusters_names[c1]);
    instance->clusters_names[c1] = calloc(required, sizeof(char));
    sprintf(instance->clusters_names[c1], "J%d", instance->initial_cluster_count - instance->current_cluster_count);

    for (int i = c2 + 1; i < clusters; ++i) {
        instance->clusters_names[i - 1] = instance->clusters_names[i - 1];
    }

    clusters_calculate_distances(instance);
}

/* Public section */
pclusters_matrix clusters_create_matrix(int *source, int size) {
    size_t matrix_type_size = sizeof(clusters_matrix);
    pclusters_matrix new_instance = (pclusters_matrix) calloc(1, matrix_type_size);

    if (new_instance) {
        // Allocation for the triangular matrix with all zeros
        int matrix_size = (size * (size - 1) / 2) + size;
        new_instance->matrix = (double *) calloc(matrix_size, sizeof(double));
        new_instance->clusters_names = (char **) calloc(size, sizeof(char *));
        new_instance->clusters_nodes = (ptree_node *) calloc(size, sizeof(ptree_node));
        if (new_instance->matrix) {
            new_instance->initial_cluster_count = size;
            new_instance->current_cluster_count = size;
            new_instance->matrix_size = matrix_size - size;
            // Fill out matrix from the source distances matrix
            for (int i = 0; i < size - 1; i++) {
                for (int j = i + 1; j < size; j++) {
                    new_instance->matrix[clusters_to_index(i, j)] = source[i + j * size];
                }
            }
            for (int i = 0; i < size; ++i) {
                int required = snprintf(NULL, 0, "C%d", i);
                new_instance->clusters_names[i] = calloc(required, sizeof(char));

                sprintf(new_instance->clusters_names[i], "C%d", i);
                new_instance->clusters_nodes[i] = clusters_create_node(new_instance, i);
            }
            clusters_calculate_distances(new_instance);
        } else {
            // In case of error
            clusters_free_distances(new_instance);
            new_instance = 0;
        }
    }
    return new_instance;
}

void clusters_free_distances(pclusters_matrix instance) {
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
    printf("Joining clusters: %s and %s\n", instance->clusters_names[c1], instance->clusters_names[c2]);

    ptree_node parent = calloc(1, sizeof(tree_node));
    double old_distance = instance->matrix[clusters_to_index(c1, c2)];
    // d(AB)/2 + [r(A) - r(B)/(2(N-2))]

    // d(AB) - S(AU) = 4
    parent->right = instance->clusters_nodes[c1];
    parent->rvalue = (old_distance / 2) +
                     ((clusters_cumulative_distance(instance, c1) - (clusters_cumulative_distance(instance, c2))) /
                      (2 * (instance->current_cluster_count - 2)));
    parent->left = instance->clusters_nodes[c2];
    parent->lvalue = old_distance - parent->rvalue;

    clusters_update_distances(instance, c1, c2, old_distance);
    char *newStr = calloc(strlen(instance->clusters_names[c1]) + 1, sizeof(char));
    strcpy(newStr, instance->clusters_names[c1]);
    parent->value = newStr;

    instance->filogenetic_tree_root = parent;
    instance->clusters_nodes[c1] = parent;
    for (int i = c2+1; i <= instance->current_cluster_count; ++i) {
        instance->clusters_nodes[i - 1] = instance->clusters_nodes[i];
    }
    instance->clusters_nodes[instance->current_cluster_count] = 0;

}

void clusters_print_matrix(pclusters_matrix matrix) {
    if (!matrix) return;

    int clusters = matrix->current_cluster_count;
    int size = matrix->initial_cluster_count;

    // Print table header
    printf("%10s|", "Clusters");
    for (int i = 0; i < clusters; i++) {
        printf("%-7s|", matrix->clusters_names[i]);

    }
    printf("\n");

    // Print table rows
    for (int i = 1; i < clusters; i++) {
        printf("%10s|", matrix->clusters_names[i]);
        for (int j = 0; j < i; j++) {
            printf("%7.2f|", matrix->matrix[clusters_to_index(j, i)]);
        }
        printf("\n");
    }

    // print last sum row
    printf("%10s|", "SUM");
    for (int j = 0; j < clusters; j++) {
        printf("%7.2f|", matrix->matrix[matrix->matrix_size + j]);
    }
    printf("\n");
}
