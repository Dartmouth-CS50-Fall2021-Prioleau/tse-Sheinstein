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
#include "../libcs50/file.h"
#include "../libcs50/memory.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"

/************************ function prototypes **************************/
void
item_delete(void *item);

/************************** main ***************************************/
int
main(int argc, char* argv[]) {

    // validate parameters
    if (argc != 3) {
        fprintf (stderr, "incorrect number of parameters\n"); // check number of params
        return 1;
    }

    // check if oldIndexFilename is the pathname of a readable file
    FILE *ofp = assertp(fopen(argv[1], "r"),"oldIndexFilename could not be opened for reading\n");

    // load the inverted index
    const int NUMSLOTS = 2*(lines_in_file(ofp)+1);
    index_t* index = index_new(NUMSLOTS); // create empty index
    if (index == NULL) {
        fprintf(stderr, "index_new failed\n");
        fclose(ofp);
        return 1;
    }

    printf("Loading index...\n");
    index = index_load(index, ofp); // load the index structure
    fclose(ofp);

    printf("Saving index...\n");
    index_save(index, argv[2]); // save the index structure
        
    index_delete(index, item_delete); // clean up
    printf("Index Loaded and Saved in Indextest.\n");
    
    return 0;
}

/*************************** local helper functions ***************************/

/**************** item_delete ****************/
/* Frees item memory of items in index ie. counterset.
 */
void
item_delete(void *item)
{
    if (item != NULL) {
        counters_delete(item); // delete the counterset
        //item = NULL;
    }
}

