#include<stdio.h>
#include<stdlib.h>
#include <levenshtein.h>
#include <assert.h>
#include "clusters_matrix.h"
#include "fasta_parser.h"
#include "binary_tree.h"
#include "cmdline.h"

int main(int argc, char **argv) {
    static struct gengetopt_args_info args_info;
    assert(cmdline_parser(argc, argv, &args_info) == 0);
    int verbose = args_info.verbose_flag;

    if(args_info.inputs_num == 0){
        // TODO: Print error
        return  -1;
    }
    char *source_file = args_info.inputs[0];
    pfasta parsed = fasta_parse_file(source_file);
    size_t count = parsed->sequences_count;

    int *distances_matrix = calloc(count * count, sizeof(int));
    int row = 0, column = 0;
    for (GSList *item = parsed->sequences; item != NULL; item = g_slist_next(item)) {
        for (GSList *item2 = parsed->sequences; item2 != NULL; item2 = g_slist_next(item2)) {
            int distance = (int) levenshtein(item->data, item2->data);
            distances_matrix[row * count + column] = distance;
            column++;
        }
        row++;
        column = 0;
    }
    fasta_free(parsed);

    pclusters_matrix matrix = clusters_create_matrix(distances_matrix, count);
    clusters_print_matrix(matrix);

    int degree = 0;
    do {
        degree = clusters_increase_clustering(matrix);
        clusters_print_matrix(matrix);
        btree_draw_horizontal(matrix->filogenetic_tree_root);
        printf("\n\n\n");
    } while (degree > 1);
    clusters_free_distances(matrix);
    free(distances_matrix);
}