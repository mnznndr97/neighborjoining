# CMakeList.txt: file di progetto CMake di primo livello. Eseguire la configurazione globale
# e includere qui i sottoprogetti.
#
cmake_minimum_required (VERSION 3.8)

PROJECT (neighborjoining)
SET(NJSOURCES 
	src/fasta_parser.c src/fasta_parser.h
	src/binary_tree.c src/binary_tree.h
	src/clusters_matrix.c src/clusters_matrix.h)

ADD_EXECUTABLE(neighborjoining-win src/neighborjoining.c ${NJSOURCES})
ADD_EXECUTABLE(neighborjoining-linux src/neighborjoining.c ${NJSOURCES})

TARGET_INCLUDE_DIRECTORIES(
	neighborjoining-win
	PRIVATE include/glib
	PRIVATE include/glib/glib)

TARGET_INCLUDE_DIRECTORIES(
		neighborjoining-linux
		PRIVATE /usr/include/glib-2.0/)