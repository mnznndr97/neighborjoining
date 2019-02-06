# CMakeList.txt: file di progetto CMake di primo livello. Eseguire la configurazione globale
# e includere qui i sottoprogetti.
#
cmake_minimum_required(VERSION 3.8)
SET(CMAKE_C_STANDARD 11)
SET(CMAKE_C_STANDARD_REQUIRED ON)

PROJECT(neighborjoining)
IF (UNIX)
    FIND_PACKAGE(PkgConfig)
    PKG_SEARCH_MODULE(GLIB glib-2.0)

ENDIF ()

SET(LSOURCE include/levenshtein)
SET(NJSOURCES
        src/fasta_parser.c src/fasta_parser.h
        src/binary_tree.c src/binary_tree.h
        src/clusters_matrix.c src/clusters_matrix.h
        src/cmdline.c src/cmdline.h
        ${LSOURCE}/levenshtein.h ${LSOURCE}/levenshtein.c)

ADD_EXECUTABLE(neighborjoining-linux src/neighborjoining.c ${NJSOURCES})
TARGET_INCLUDE_DIRECTORIES(neighborjoining-linux PRIVATE include/levenshtein)

set_property(TARGET neighborjoining-linux PROPERTY C_STANDARD 11)
IF (UNIX)
    target_include_directories(neighborjoining-linux PRIVATE ${GLIB_INCLUDE_DIRS})
    target_link_libraries(neighborjoining-linux PRIVATE ${GLIB_LIBRARIES})
ENDIF ()
