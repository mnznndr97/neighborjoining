#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "fasta_parser.h"

/**
 * Substitutes the first occurrence of the characters [\n \r] with the \0 char
 *
 * @param str Source buffer
 */
static void line_replace_trailing(char *str) {
    char *pos;
    if ((pos = strchr(str, '\n')) != NULL)
        *pos = '\0';
    if ((pos = strchr(str, '\r')) != NULL)
        *pos = '\0';
}

/**
 * Parses a single FASTA sequence inside a file
 *
 * @param file_handle Handle of the opened file
 * @param line_buffer Buffer used for read the file (to avoid more allocations)
 * @param lineb_length Length of the buffer
 * @return Parsed FASTA sequence
 */
static char *fasta_parse_sequence(FILE *file_handle, char **line_buffer, size_t *lineb_length) {
    GSList *lines_list = NULL;
    ssize_t readed_chars = 0;
    size_t sequence_length = 0;
    char *buffer;

    while ((readed_chars = getline(line_buffer, lineb_length, file_handle)) != -1) {
        // The buffer returner from getline contains the newline escape characters. We first need to remove them
        line_replace_trailing(*line_buffer);
        // We have encountered an empty line. The sequence is finished
        if (strlen(*line_buffer) <= 0) break;

        // We need to copy the buffer before the next getline
        char *localb = calloc(readed_chars + 1, sizeof(char));
        strcpy(localb, *line_buffer);

        lines_list = g_slist_append(lines_list, localb);
        sequence_length += readed_chars;
    }

    // We must have read at least one line of the sequence, otherwise the file is not valid
    assert(sequence_length > 0);

    // We now only have to merge the read lines
    buffer = calloc(sequence_length + 1, sizeof(char));
    for (GSList *item = lines_list; item != NULL; item = g_slist_next(item)) {
        strcat(buffer, item->data);
        free(item->data);
    }
    g_slist_free(lines_list);
    return buffer;
}

/**
 * Parses all sequences FASTA file into a structure
 *
 * @param file_name File name of the file to parse
 * @return fasta structure with the parsed sequences list
 */
pfasta fasta_parse_file(const char *file_name) {
    GSList *seq_list = NULL;
    char *curr_line = NULL;
    size_t line_length = 0;

    FILE *file_handle = fopen(file_name, "r");
    if (!file_handle) return NULL;
    pfasta result = malloc(sizeof(fasta));
    if (!result) return NULL;

    while (getline(&curr_line, &line_length, file_handle) != -1) {
        if (curr_line[0] == '>') {
            // We are reading a sequence
            char *seq_buffer = fasta_parse_sequence(file_handle, &curr_line, &line_length);
            seq_list = g_slist_append(seq_list, seq_buffer);
        }
    }
    free(curr_line);
    fclose(file_handle);

    result->sequences = seq_list;
    result->sequences_count = g_slist_length(seq_list);
    return result;
}

/**
 * Frees al the resources associated to a fasta structure
 *
 * @param fasta_seq Pointer to the fasta structure
 */
void fasta_free(pfasta fasta_seq) {
    if (!fasta_seq) return;

    if (fasta_seq->sequences) {
        for (GSList *item = fasta_seq->sequences; item != NULL; item = g_slist_next(item)) {
            free(item->data);
        }
        g_slist_free(fasta_seq->sequences);
    }
    memset(fasta_seq, 0, sizeof(fasta));
    free(fasta_seq);
}

