#ifndef _FASTA_PARSER_H
#define _FASTA_PARSER_H

#include <glib.h>

typedef struct _fasta_sequences
{
    GSList* sequences; ///< List of all sequences read from a FASTA file
    size_t sequences_count; ///< Number of parsed sequences
} fasta, *pfasta;

pfasta fasta_parse_file(const char *file_name);
void fasta_free(pfasta fasta_seq);

#endif