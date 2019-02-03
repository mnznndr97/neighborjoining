#ifndef _FASTA_PARSER_H
#define _FASTA_PARSER_H

#include <glib.h>

typedef struct _fasta_sequences
{
    GSList* sequences;
    size_t sequences_count;
} fasta, *pfasta;

pfasta fasta_parse_file(const char *file_name);
void fasta_free(pfasta fasta);

#endif