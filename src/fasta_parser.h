#ifndef _FASTA_PARSER_H
#define _FASTA_PARSER_H

typedef struct _fasta_sequences fasta, *pfasta;

#include <glib.h>

/**
 * Parses all sequences FASTA file into a structure
 *
 * @param file_name File name of the file to parse
 * @return fasta structure with the parsed sequences list
 */
pfasta fasta_parse_file(const char *file_name);

/**
 * Frees al the resources associated to a fasta structure
 *
 * @param fasta_seq Pointer to the fasta structure
 */
void fasta_free(pfasta fasta_seq);

/**
 * Returns the number of parsed sequences fromt he fasta file
 * @param fasta_seq Pointer to the fasta structure
 * @return Number of parsed sequences
 */
size_t fasta_sequences_count(const fasta* fasta_seq);

/**
 * Returns the number of parsed sequences fromt he fasta file
 * @param fasta_seq Pointer to the fasta structure
 * @return Number of parsed sequences
 */
GSList* fasta_sequences_list(const fasta* fasta_seq);

#endif