#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "fasta_parser.h"

/**
 * @brief Rimuove le prime occorrenze di tutti i caratteri di ritorno a capo
 * Rimuove le prime occorrenze di tutti i caratteri di ritorno a capo [\n \r] con il carattere \0
 *
 * @param str Buffer da modificare
 */
static void line_replace_trailing(char* str){
    char* pos;
    if ((pos=strchr(str, '\n')) != NULL)
        *pos = '\0';
    if ((pos=strchr(str, '\r')) != NULL)
        *pos = '\0';
}

static char* fasta_parse_sequence(FILE* file_handle, char* line_buffer, size_t* lineb_length){
    GSList* lines_list = NULL;
    ssize_t readed_chars = 0;
    size_t sequence_length = 0;
    char* buffer;

    while (((readed_chars = getline(&line_buffer, lineb_length, file_handle)) != -1)
            && strlen(line_buffer) > 1){

        char * localb = malloc(readed_chars + 1);
        strcpy(localb, line_buffer);
        line_replace_trailing(localb);

        lines_list = g_slist_append(lines_list, localb);
        sequence_length += readed_chars;
    }

    assert(sequence_length > 0);
    buffer = malloc(sequence_length);
    for (GSList *item = lines_list; item != NULL; item = g_slist_next(item)) {
        strcat(buffer, item->data);
        free(item->data);
    }
    g_slist_free(lines_list);
    return buffer;
}

pfasta fasta_parse_file(const char *file_name) {
    FILE *file_handle = fopen(file_name, "r");

    pfasta  result = malloc(sizeof(fasta));
    GSList* seq_list = NULL;
    char *curr_line = NULL;
	size_t line_length = 0;
	ssize_t readed_chars;

    while ((readed_chars = getline(&curr_line, &line_length, file_handle)) != -1) {
        if (curr_line[0] == '>'){
            // We are reading a sequence
            char* seq_buffer = fasta_parse_sequence(file_handle, curr_line, &line_length);
            seq_list = g_slist_append(seq_list, seq_buffer);
        }
    }
    fclose(file_handle);

    result->sequences = seq_list;
    return  result;
}

void fasta_free(pfasta fasta){
    if(!fasta) return;

    if(fasta->sequences) {
        for (GSList *item = fasta->sequences; item != NULL; item = g_slist_next(item)) {
            free(item->data);
        }
        g_slist_free(fasta->sequences);
        fasta->sequences = NULL;
    }
}

