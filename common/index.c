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
#include "../libcs50/memory.h"
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"


/*********************** file-local global variables ****************/
/* none */

/************************* global functions *************************/
/* that is, visible outside this file */
/* see index.h for comments about exported functions */


/********************* local functions ***********************/
/* not visible outside this file */
static void get_key_item_pairs(void *arg, const char *key, void *item);
static void print_key_item_pairs(void *arg, const int key, const int count);

/*********************** global types ***********************/
typedef hashtable_t index_t;

/*********************** global functions *********************/
/* that is, visible outside this file */
/* see index.h for comments about exported functions */


/************************* index_new() ************************/
/* see index.h for description */
index_t*
index_new(const int num_slots)
{
    return (index_t*)hashtable_new(num_slots);
}


/************************* index_insert() ***********************/
/* see index.h for description */
bool
index_insert(index_t* index, const char *key, void *item)
{
    return (index_t*)hashtable_insert(index, key, item);
}

/****************************** index_find ****************************/
/* see index.h for description */
void*
index_find(index_t* index, const char* key)
{
    return (index_t*)hashtable_find(index, key);
}


/**************** index_print() ****************/
/* see index.h for description */
void index_print(index_t *index, void *fp, void (*itemprint)(void *fp, const char *key, void *item))
{
    hashtable_print(index,fp, itemprint);
}


/************************* index_iterate() ***********************/
/* see index.h for description */
void
index_iterate(index_t* index,  void *arg, void (*itemfunc)(void *arg, const char *key, void *item)){
    hashtable_iterate(index, arg, itemfunc);
}


/************************* index_delete() ************************/
/* see index.h for description */
void
index_delete(index_t *index, void (*itemdelete)(void *item) )
{
    hashtable_delete(index, itemdelete);
}


/************************** index_build() ************************/
/* see index.h for description */
index_t*
index_build(index_t *index, char *dir) {

    int dirlen = strlen(dir);
    int docID = 1; // first file    
    char ifile[dirlen + 50]; // assume docID will be no more than 29 digits...
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
                            if ((ctrs = index_find(index, result)) == NULL)
                            {
                                ctrs = counters_new(); // create counter set
                                if (ctrs != NULL)
                                {
                                    counters_add(ctrs, docID);
                                    index_insert(index, result, ctrs);
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
    
    #ifdef MEMTEST
         count_report(stdout, "After index_build");
    #endif 

    return(index);  
}

/**************************** index_save() *************************/
/* see index.h for description */
void
index_save(index_t *index, char *filename)
{
    FILE *fp = fopen(filename, "w"); // open indexfile, provided by user
    if (fp != NULL) {
        index_iterate(index, (void*)fp, get_key_item_pairs); // go through the index
        fclose(fp);
    }
    else fprintf(stderr, "error opening newIndexFilename\n");

    #ifdef MEMTEST
         count_report(stdout, "After index_save");
    #endif
}


/**************** index_load() ****************/
/* see index.h for description */
index_t*
index_load(index_t *index, FILE *fp)
{
    int docID, count;
    char *word;
    while ((word = freadwordp(fp)) != NULL) { // get word
        counters_t *ctrs = counters_new(); // create new counterset for each word
        if (ctrs != NULL) {
            while (fscanf(fp, "%d %d", &docID, &count) == 2) { // scan until end of line
                counters_set(ctrs, docID, count); // put docID count pair into a counterset
            }
            index_insert(index, word, ctrs); // put word and all associated docID count pairs into ht
            free(word);
        }
        else fprintf(stderr, "counters_new failed\n"); 
    }
    #ifdef MEMTEST
         count_report(stdout, "After index_load");
    #endif
    return index;
}



/************************** module local function descriptions and implementations ***************************/

/**************** get_key_item_pairs() ****************/
/* Prints a key (a word) from the index to 
 * indexfile (which may or may not exist – if the file
 * already exists, its contents will be overwritten),
 * and iterates over the counterset which corresponds 
 * to that key.
 * The 'static' modifier means this function is not visible
 * outside this file 
 */
static void
get_key_item_pairs(void *arg, const char *key, void *item) 
{   
    fprintf((FILE*)arg, "%s", key); // print word to file
    counters_iterate(item, arg, print_key_item_pairs); // iterate over counterset
    fprintf((FILE*)arg, "\n"); // print newline for next word
}

/********************* print_key_item_pairs **************************/
/* Prints [docID count] pairs from a counterset
 * to indexfile (which may or may not exist – if the file
 * already exists, its contents will be overwritten),
 * and iterates over the counterset which corresponds 
 * to that key.
 * The 'static' modifier means this function is not visible
 * outside this file 
 */
static void
print_key_item_pairs(void *arg, const int key, const int count)
{
    fprintf((FILE*)arg, " %d %d", key, count); // print [docID count] pairs to file
}

/********************************************************
 **************** unit testing **************************
 ********************************************************/


#ifdef UNIT_TEST
#include < stdbool.h>
#include "../libcs50/memory.h"
#include"../libcs50/counters.h"

#include "unittest.h" 
// file-local global  varialbes
static int unit_tested = 0; // number of test cases run
static int unit_failed = 0; // number of test caser run

// a macro for  shorthand calls to expect()
#define EXPECT(cond) {unit_expect((cond), __LINE__); }

// Checks "condition", increments unit_tested, print FAILS or PASS
void
unit_expect(bool condition, int linenum)
{
    unit_tested++;
    if (condition){
        printf("PASS test %03d at line %d\n", unit_tested, linenum);
    }else{
        printf("FAIL test %03d at line %d\n", unit_tested, linenum);
        unit_failed++;
    }
}

/************************************* main () ************************************/
int
main()
{
    // make index to work with
    index_t* index1 = index_new(7);
    EXPECT(index1 != NULL);
    EXPECT(index1-> size > 0);

    // insert  5 things to the index_t index1
    index_insert(index1,"Princeton",1);
    index_insert(index1,"Harvard",2);
    index_insert(index1,"Yale",3);
    index_insert(index1,"Columbia",3);
    index_insert(index1,"Upenn",4);
    index_insert(index1,"Dartmouth",5);

    // check the number of key-item pairs in index
    int index_count = 0;
    EXPECT(index_iterate(index1, &index_count,itemcount) == 5);

    // try looking for a key present in the index and onother  absent
    EXPECT(index_find(index1,"Columbia") != NULL);
    EXPECT(index_find(index1,"Cornell") == NULL);
    
    // add one more key-value pair to index
    index_insert(index1,"Cornell",6);
    
    // check that the number of items has increased to 6
    index_count = 0;
    EXPECT(index_iterate(index1, &index_count,itemcount) == 6);
   
   // try looking for Cornell again. This time around, it should be present
   EXPECT(index_find(index1,"Cornell") != NULL);

   // delete all the items
   index_delete(index1,itemdelete);

   //check number of frees after deleting
   EXPECT(count_net() == 0); // count_net is a function from memory.h 


/* ===========test  index_build, index_save, and index_load with realtime directory with webpages ==================== */
    
   // make index to work with
    index_t* index2 = assertp(index_new(200),"index_new failed\n") ;
    EXPECT(index2 != NULL);
    EXPECT(index2->size > 200);

    // build an index with ../tse-output/letters-depth-3 crawler produced directory and count
    int new_index_count = 0;
    index2 = index_build(index2, ../tse-output/letters-depth-3);
    EXPECT(count_report() !=0);

    // save the index
    index_save(index2, ../tse-output/index-save-letters-depth-3);
    EXPECT(count_report() != 0);
    
    // load the index
    FILE *ofp = assertp(fopen(argv[1], "r"),"oldIndexFilename could not be opened for reading\n");
    index_t* index3 = assertp(index_new(2*(lines_in_file(../tse-output/index-save-letters-depth-3)+1)),"index_new failed\n");
    index_load(index3,../tse-output/index-load-letters-depth-3) ;
    EXPECT(count_report() != 0);

    // clean up
    index_delete(index, item_index_delete); // clean up
    EXPECT(count_net() == 0);
    // check for frees after calls




   // print a summary
   if(unit_failed >0){
       printf(" FAILED %d of %d tests\n", unit_failed, unit tested);
       return unit_failed;     
   }else{
       printf("PASSED all of %d tests\n", unit_tested);
       return 0;
   }
}



/* count the non-null items in the set.
 * note here we don't care what kind of item is in set.
 */
static void itemcount(void* arg, const char* key, void* item)
{
  int* nitems = arg;

  if (nitems != NULL && key!= NULL && item != NULL){
    (*nitems)++;
  }
}

// delete an item  in index
static void itemdelete(void* item)
{
  if (item != NULL) {
    //free(item);   
  }

}   

/**************** item_delete ****************/
/* Frees item memory of items in index ie. counterset.
 */
void
item_index_delete(void *item)
{
    if (item != NULL) {
        counters_delete(item); // delete the counterset
        //item = NULL;
    }
}

#endif // UNIT_TEST

