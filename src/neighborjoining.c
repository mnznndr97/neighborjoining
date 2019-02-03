#include<stdio.h>
#include<stdlib.h>
#include "clusters_matrix.h"
#include "fasta_parser.h"
#include <levenshtein.h>

int main(int argc, char **argv) {

//	int* source_matrix = (int*)calloc(6 * 6, sizeof(int));
//
//	source_matrix[0] = 0;
//	source_matrix[6] = 5; source_matrix[7] = 0;
//	source_matrix[12] = 4; source_matrix[13] = 7; source_matrix[14] = 0;
//	source_matrix[18] = 7; source_matrix[19] = 10; source_matrix[20] = 7; source_matrix[21] = 0;
//	source_matrix[24] = 6; source_matrix[25] = 9; source_matrix[26] = 6; source_matrix[27] = 5; source_matrix[28] = 0;
//	source_matrix[30] = 8; source_matrix[31] = 11; source_matrix[32] = 8; source_matrix[33] = 8; source_matrix[34] = 8; source_matrix[35] = 0;
//
//	pclusters_matrix matrix = clusters_create_matrix(source_matrix, 6);
//	clusters_print_matrix(matrix);
//
//	system("pause");
//	clusters_increase_clustering(matrix);
//	clusters_print_matrix(matrix);
//
//	clusters_increase_clustering(matrix);
//	clusters_print_matrix(matrix);
//
//	clusters_increase_clustering(matrix);
//	clusters_print_matrix(matrix);
//
//
//	clusters_increase_clustering(matrix);
//	clusters_print_matrix(matrix);
//
//
//	clusters_free_distances(matrix);

    pfasta parsed = fasta_parse_file(argv[1]);
    size_t count = parsed->sequences_count;

    int* distances_matrix = calloc(count * count, sizeof(int));
    int row = 0, column = 0;
    for (GSList *item = parsed->sequences; item != NULL; item = g_slist_next(item)) {
        for (GSList *item2 = parsed->sequences; item2 != NULL; item2 = g_slist_next(item2)) {
            int distance = (int)levenshtein(item->data, item2->data);
            distances_matrix[row * count + column] = distance;
            printf("%s\n%s\n%d[%d,%d -> %d]\n\n", (char *) item->data, (char *) item2->data, distance, row, column, row * count + column);
            column++;
        }
        row++;
        column = 0;
    }
    fasta_free(parsed);

    pclusters_matrix matrix = clusters_create_matrix(distances_matrix, count);
    clusters_print_matrix(matrix);

    clusters_increase_clustering(matrix);
    clusters_print_matrix(matrix);
}

void neighbor_joining(int *matrix, int n) {
    pclusters_matrix instance = clusters_create_matrix(matrix, 6);

    while (clusters_increase_clustering(instance) > 1) {
        // Build tree
    }
    // print tree
}