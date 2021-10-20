/* 
 * index.c - CS50 'index' module
 *
 * see index.h for more information
 * 
 * Salifyanji J Namwila October 19, 2021.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "word.h"
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"



/**************** local functions ****************/
/* not visible outside this file */
static void get_keypairs(void *arg, const char *key, void *item);
static void print_keypairs(void *arg, const int key, const int count);

/**************** functions ****************/

/**************** index_build ****************/
hashtable_t* index_build(hashtable_t *ht, char *dir) {

    int dirlen = strlen(dir);
    int docID = 1; // first file    
    char ifile[dirlen + 30]; // assume docID will be no more than 29 digits...
    sprintf(ifile, "%s/%d", dir, docID); // create path
    FILE *fp = fopen(ifile, "r");

    while (fp != NULL) // while there are still files to read in the directory
    { 
        char *url = freadlinep(fp);
        if (url != NULL)
        {
            int depth;
            fscanf(fp, "%d\n", &depth);
            char *html = freadfilep(fp);
            if (html != NULL) {
                webpage_t *page = webpage_new(url, depth, html);
                if (page != NULL)
                {
                    int pos = 0;
                    char *result; // words found in document

                    while ((result = webpage_getNextWord(page, &pos)) != NULL) // actually build the index
                    { 
                        if (strlen(result) >= 3)
                        {
                            result = normalize_word(result);

                            counters_t *ctrs;
                            // if word not already in index, create new counterset
                            if ((ctrs = hashtable_find(ht, result)) == NULL)
                            {
                                ctrs = counters_new(); // create counter set
                                if (ctrs != NULL)
                                {
                                    counters_add(ctrs, docID);
                                    hashtable_insert(ht, result, ctrs);
                                }
                                else fprintf(stderr, "counters_new failed\n"); 
                            }
                            // if word already in index, update count for corresponding docID
                            else counters_add(ctrs, docID); // increment existing counterset found in ht
                        }
                        free(result);
                    }

                    webpage_delete(page); // clean up
                }
            }
        }
        fclose(fp);
        
        docID++;
        sprintf(ifile, "%s/%d", dir, docID); // create new path
        fp = fopen(ifile, "r"); // open next document in the directory
    }
    
    return(ht);    
}

/**************** index_save ****************/
void index_save(hashtable_t *ht, char *filename)
{
    FILE *fp = fopen(filename, "w"); // open indexfile, provided by user
    if (fp != NULL) {
        hashtable_iterate(ht, (void*)fp, get_keypairs); // go through the hashtable
        fclose(fp);
    }
    else fprintf(stderr, "error opening newIndexFilename\n");
}

/**************** get_keypairs ****************/
/* Prints a key (a word) from the index hashtable to 
 * indexfile (which may or may not exist – if the file
 * already exists, its contents will be overwritten),
 * and iterates over the counterset which corresponds 
 * to that key.
 * The 'static' modifier means this function is not visible
 * outside this file 
 */
static void get_keypairs(void *arg, const char *key, void *item) 
{   
    fprintf((FILE*)arg, "%s", key); // print word to file
    counters_iterate(item, arg, print_keypairs); // iterate over counterset
    fprintf((FILE*)arg, "\n"); // print newline for next word
}

/**************** print_keypairs ****************/
/* Prints [docID count] pairs from a counterset
 * to indexfile (which may or may not exist – if the file
 * already exists, its contents will be overwritten),
 * and iterates over the counterset which corresponds 
 * to that key.
 * The 'static' modifier means this function is not visible
 * outside this file 
 */
static void print_keypairs(void *arg, const int key, const int count)
{
    fprintf((FILE*)arg, " %d %d", key, count); // print [docID count] pairs to file
}

/**************** index_load ****************/
hashtable_t* index_load(hashtable_t *ht, FILE *fp)
{
    int docID, count;
    char *word;
    while ((word = freadwordp(fp)) != NULL) { // get word
        counters_t *ctrs = counters_new(); // create new counterset for each word
        if (ctrs != NULL) {
            while (fscanf(fp, "%d %d", &docID, &count) == 2) { // scan until end of line
                counters_set(ctrs, docID, count); // put docID count pair into a counterset
            }
            hashtable_insert(ht, word, ctrs); // put word and all associated docID count pairs into ht
            free(word);
        }
        else fprintf(stderr, "counters_new failed\n"); 
    }
    return ht;
}
