#include<stdio.h>
#include<stdlib.h>
#include <levenshtein.h>
#include <assert.h>
#include "clusters_matrix.h"
#include "fasta_parser.h"
#include "cmdline.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main(int argc, char **argv) {
    static struct gengetopt_args_info args_info;
    assert(cmdline_parser(argc, argv, &args_info) == 0);
    int verbose = args_info.verbose_flag;
    if (args_info.inputs_num == 0) {
        fprintf(stderr, "FASTA input file not specified");
        return -1;
    }

    char *source_file = args_info.inputs[0];
    pfasta parsed = fasta_parse_file(source_file);
    if (!parsed || fasta_sequences_count(parsed) == 0) {
        fasta_free(parsed);
        perror("Unable to parse FASTA file");
        return -1;
    }

    size_t count = fasta_sequences_count(parsed);
    int *distances_matrix = calloc(count * count, sizeof(int));
    if (!distances_matrix) {
        fasta_free(parsed);
        perror("Unable to create distance matrix");
        return -1;
    }

    int row = 0, column = 0;
    for (GSList *item = fasta_sequences_list(parsed); item != NULL; item = g_slist_next(item)) {
        for (GSList *item2 = fasta_sequences_list(parsed); item2 != NULL; item2 = g_slist_next(item2)) {
            int distance = (int) levenshtein(item->data, item2->data);
            distances_matrix[row * count + column] = distance;
            column++;
        }
        row++;
        column = 0;
    }
    fasta_free(parsed);

    pclusters_matrix matrix = clusters_create_matrix(distances_matrix, count, verbose);
    if (!matrix) {
        free(distances_matrix);
        perror("Unable to create clustering instance");
        return -1;
    }
    while (clusters_get_count(matrix) > 1) {
        clusters_increase_clustering(matrix);
    }
    printf(ANSI_COLOR_YELLOW "Phylogenetic tree:" ANSI_COLOR_RESET "\n");
    btree_draw_horizontal(clusters_get_tree(matrix));
    clusters_free_distances(matrix);
    free(distances_matrix);
}