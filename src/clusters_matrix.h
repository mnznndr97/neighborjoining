#ifndef _CLUSTERS_MATRIX_H
#define _CLUSTERS_MATRIX_H

#include "binary_tree.h"

/**
* Contenitore per i dati sui clusters
**/
typedef struct _clusters_matrix
{
	double *matrix;
	char** clusters_names;
	int initial_cluster_count;
	int current_cluster_count;
	int matrix_size;

	ptree_node filogenetic_tree_root;
	ptree_node *clusters_nodes;
} clusters_matrix, *pclusters_matrix;


/**
* Crea una nuova matrice delle distanze tra cluster
* Viene inizialmente creato un cluster per ogni individio presente nella matrice
* in ingresso
*
* @param source Matrice delle distanze in ingresso
* @param size Numero di individui nella matrice di ingresso
* @return Istanza del clustering
**/
pclusters_matrix clusters_create_matrix(int* source, int size);

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
int clusters_increase_clustering(pclusters_matrix matrix);

/**
* Stampa a video la matrice di cluster nello stato corrente
* @param matrice Matrice di clusters
**/
void clusters_print_matrix(pclusters_matrix instance);
#endif // !_CLUSTERS_MATRIX_H
