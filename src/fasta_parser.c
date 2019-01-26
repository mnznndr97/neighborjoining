#include "io.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <glib.h>


const char * load_file(const char *file_name) {
    FILE *f = fopen(file_name, "r");

    GSList* list1 = NULL;
    GSList* list2 = NULL;

    char *result = NULL;

    char *line1 = NULL;
	size_t len1 = 0;
	ssize_t read1;

    char *line2 = NULL;
	size_t len2 = 0;
	ssize_t read2;

    size_t lenTotal = 0;

    if (!f) {
        perror("Unable to open input file");
        return NULL;
    }
    
    while ((read1 = getline(&line1, &len1, fp1)) != -1) {
        if (line1[0] == '>'){
            while ((read2 = getline(&line2, &len2, fp2)) != -1) {
                if(line2[0] != ''){
                    list2 = g_slist_append(list2, line2);
                    lenTotal += len2;
                }
                else{
                    break;
                }    
            }
        }

        if (line1[0] == ' ') {
            for (GList *item = list2; item != NULL; item = g_slist_next(l)) {
                result = (char *) malloc(1 + lenTotal);
                char * st = (item->data);
                strcat(result, st);
            }

            g_slist_free(list2);

            list1 = g_slist_append(list1, result);
		} 
    }

    fclose(f);

    if (line1)
		free(line1);
    if (line2)
		free(line2);
}

