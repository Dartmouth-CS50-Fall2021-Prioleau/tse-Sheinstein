/* 
 * indextest.c - second driver for `index` module
 *
 * The indextest reads an index file from indexer and saves the 
 * recreated index data structure into another file
 * 
 * see README for more details.
 *
 * Salifyanji J Namwila October 19, 2021.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../common/index.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"

//function prototypes
void item_delete();

int main(int argc, char* argv[]) {

    // validate parameters
    if (argc != 3) {
        fprintf (stderr, "incorrect number of parameters\n"); // check number of params
        return 1;
    }

    // check if oldIndexFilename is the pathname of a readable file
    FILE *ofp = fopen(argv[1], "r");
    if (ofp == NULL) {
        fprintf (stderr, "oldIndexFilename could not be opened for reading\n");
        return 1;
    }
    else { // load the inverted index
        const int NUMSLOTS = 500;
        hashtable_t* index = hashtable_new(NUMSLOTS); // create empty hashtable
        if (index == NULL) {
            fprintf(stderr, "hashtable_new failed\n");
            fclose(ofp);
            return 1;
        }

        printf("Loading index...\n");
        index = index_load(index, ofp); // load the index structure
        fclose(ofp);

        printf("Saving index...\n");
        index_save(index, argv[2]); // save the index structure
        
        hashtable_delete(index, item_delete); // clean up
        printf("Done.\n");
    }

    return 0;
}

/**************** item_delete ****************/
/* Frees item memory in hashtable, in this case,
 * a counterset.
 */
void item_delete(void *item)
{
    if (item != NULL) {
        counters_delete(item); // delete the counterset
        item = NULL;
    }
}
