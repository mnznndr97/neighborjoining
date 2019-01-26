#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "fasta_parser.h"

static char* fasta_parse_sequence(FILE* file_handle, char* line_buffer, size_t* lineb_length){
    GSList* lines_list = NULL;
    ssize_t readed_chars = 0;
    ssize_t sequence_length = 0;
    char* buffer;

    while (((readed_chars = getline(&line_buffer, lineb_length, file_handle)) != -1)
            && strlen(line_buffer) > 0){
        // Need a buffer copy
        lines_list = g_slist_append(lines_list, line_buffer);
        sequence_length += readed_chars;
    }

    assert(sequence_length > 0);
    buffer = malloc(sequence_length);
}

pfasta fasta_parse_file(const char *file_name) {
    FILE *file_handle = fopen(file_name, "r");

    GSList* list1 = NULL;
    GSList* list2 = NULL;

    char *result = NULL;

    char *curr_line = NULL;
	size_t line_length = 0;
	ssize_t readed_chars;


    size_t lenTotal = 0;

    while ((readed_chars = getline(&curr_line, &line_length, file_handle)) != -1) {
        if (curr_line[0] == '>'){
            // We are reading a sequence
            fasta_parse_sequence(&list1, file_handle, curr_line, &line_length);

        }

        if (curr_line[0] == ' ') {
            for (GList *item = list2; item != NULL; item = g_slist_next(l)) {
                result = (char *) malloc(1 + lenTotal);
                char * st = (item->data);
                strcat(result, st);
            }

            g_slist_free(list2);

            list1 = g_slist_append(list1, result);
		} 
    }

    fclose(file_handle);

    if (curr_line)
		free(curr_line);
    if (line2)
		free(line2);
}

void fasta_free(pfasta fasta){
    if(!fasta);

    if(fasta->sequences) {
        g_list_free(fasta->sequences);
        fasta->sequences = NULL;
    }
}

