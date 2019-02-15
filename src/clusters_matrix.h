#ifndef _CLUSTERS_MATRIX_H
#define _CLUSTERS_MATRIX_H

#include <glob.h>
#include "binary_tree.h"

/**
* Contenitore per i dati sui clusters
**/
typedef struct _clusters_matrix clusters_matrix, *pclusters_matrix;


/**
* Crea una nuova matrice delle distanze tra cluster
* Viene inizialmente creato un cluster per ogni individio presente nella matrice
* in ingresso
*
* @param source Matrice delle distanze in ingresso
* @param size Numero di individui nella matrice di ingresso
* @return Istanza del clustering
**/
pclusters_matrix clusters_create_matrix(int *source, size_t size, int verbose_flag);

/**
* Libera le risorse associate alla matrice di clusters
* @param matrix Matrice di clusters da liberare
**/
void clusters_free_distances(pclusters_matrix matrix);

/**
* Attempts to increase the degree of clustering and returns the number of new clusters
* @param instance 
* @return Numbers of clusters after the reduction
**/
void clusters_increase_clustering(pclusters_matrix matrix);

/**
 * Gets the actual number of clusters in the current instance
 *
 * @param instance Current clustering instance
 * @return Number of clusters
 */
size_t clusters_get_count(const clusters_matrix* instance);

/**
 * Gets the actual phylogenetic tree of the current instance
 *
 * @param instance Current clustering instance
 * @return Phylogenetic tree
 */
pbtree_node clusters_get_tree(const clusters_matrix* instance);


/**
 * Print the current distance matrix for the clustering instance
 *
 * @param instance Clustering instance
 */
void clusters_print_matrix(const clusters_matrix* instance);
#endif // !_CLUSTERS_MATRIX_H
