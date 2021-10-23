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




/************************* index_new() ************************/
/* see index.h for description */
index_t*
index_new(const int num_slots)
{
    return (index_t*)hashtable_new(num_slots);
}


/************************* index_insert() ********************************/
/* see index.h for description */
bool
index_insert(index_t* index, const char *key, void *item)
{
    return (index_t*)hashtable_insert(index, key, item);
}

/****************************** index_find ********************8*************/
/* see index.h for description */
void*
index_find(index_t* index, const char* key)
{
    return (index_t*)hashtable_find(index, key);
}


/***************************** index_print() ********************************/
/* see index.h for description */
//void index_print(index_t *index, void *fp, void (*itemprint)(void *fp, const char *key, void *item))
//{
//    hashtable_print(index,fp, itemprint);
//}


/******************************* index_iterate() ********************************/
/* see index.h for description */
void
index_iterate(index_t* index,  void *arg, void (*itemfunc)(void *arg, const char *key, void *item)){
    hashtable_iterate(index, arg, itemfunc);
}


/****************************** index_delete() *******************************/
/* see index.h for description */
void
index_delete(index_t *index, void (*itemdelete)(void *item) )
{
    hashtable_delete(index, itemdelete);
}


/**************************8**** index_build() *****************************/
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

/****************************8**** index_save() *************8******************/
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


/**************************** index_load() ****************8**********/
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
            index_insert(index, word, ctrs); // put word and all associated docID count pairs into index
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

/*************************************************************************************
 ******************************** unit testing ***************************************
 ***********************************************************************************/
#ifdef UNIT_TEST
#include "unittest.h"

/*************************** static helper function prototypes defined here ************/
static void itemcount(void* arg, const char* key, void* item);
static void itemdelete(void* item);
static void item_index_delete(void *item);


/* ==================== testing index_new =================*/
/////////////////////////////////////
// make index to work with
int test_newindex0()
{
    START_TEST_CASE("newindex0");
    index_t* index0 = index_new(3);
    EXPECT(index0 != NULL);
    //EXPECT(index0-> size ==  3);
    
    EXPECT(index_find(index0, "Dartmouth") == NULL);

    index_delete(index0, NULL);
    EXPECT(count_net() == 0);

    END_TEST_CASE;
    return TEST_RESULT;
}
/* =================== ordinary test of index_new, index_insert, index_find, index_delete ===============*/
/////////////////////////////////////
//  insert  5 things to the index_t index1
int test_newindex1()
{
    START_TEST_CASE("newindex1");
    index_t* index1 = index_new(7);
    EXPECT(index1 != NULL);
    //EXPECT(index1-> size ==  7);
    
    EXPECT(index_find(index1, "Harvard") == NULL);

    //char* Princeton = "Princeton"; char* Harvard = "Harvard"; char* Yale = "Yale"; char* Columbia = "Columbia"; char* Upenn = "Upenn";
    int p  = 1 ;  int h = 2; int y = 3 ;  int c = 4; int u = 5;
    int index_count = 0;
    EXPECT(index_insert(index1,"Princeton",&p) == true);
    index_iterate(index1, &index_count,itemcount);
    EXPECT(index_count == 1);
    EXPECT(index_insert(index1,"Harvard",&h) == true);
    index_count = 0;
    index_iterate(index1, &index_count,itemcount);
    EXPECT(index_count == 2)
    EXPECT(index_insert(index1,"Yale",&y) == true);
    index_count = 0;
    index_iterate(index1, &index_count,itemcount);
    EXPECT(index_count ==3);
    EXPECT(index_insert(index1,"Columbia",&c) == true);
    index_count = 0;
    index_iterate(index1, &index_count,itemcount);
    EXPECT(index_count ==4)

    EXPECT(index_insert(index1,"Upenn",&u) == true);
    index_count = 0;
    index_iterate(index1, &index_count,itemcount);
    EXPECT(index_count ==5)

    // try looking for a key present in the index and onother  absent
    EXPECT(index_find(index1,"Columbia") == &c);
    EXPECT(index_find(index1,"Dartmouth") == NULL);
    EXPECT(index_find(index1,NULL) == NULL);

    //check for the number of items in index after finding some
    index_count = 0;
    index_iterate(index1, &index_count,itemcount);
    EXPECT(index_count ==5)

    // add one more key-value pair to index
    //char* Dartmouth = "Dartmouth";
    int d = 6;
    index_count = 0;
    EXPECT(index_insert(index1,"Dartmouth", &d) == true);

    //check for the number of items in index after finding some
    index_count = 0;
    index_iterate(index1, &index_count,itemcount);
    EXPECT(index_count == 6);

    // try looking for Cornell again. This time around, it should be present
   EXPECT(index_find(index1,"Dartmouth") == &d);

   // delete all the items
   index_delete(index1, itemdelete);
   EXPECT(count_net() == 0);
   END_TEST_CASE;
   return TEST_RESULT;

}

/*============================= test_newindex2() ================================*/
///////////////////////////////////////
//test on actual output webpages in already existing directories
int test_newindex2(){
    START_TEST_CASE("newindex2");

    // make new index
    index_t* index2 = index_new(50);
    EXPECT(index2 != NULL);
    EXPECT(index_find(index2, "Dartmouth") == NULL);
    
    /*=====test  index_build, index_save, and index_load with realtime directory with webpages ====== */

     // build an index with ../tse-output/letters-depth-3 crawler produced directory and count
    //int new_index_count = 0;
    char* old_dir = "../tse-output/letters-depth-1";
    index2 = index_build(index2,old_dir);
    EXPECT(count_net() != 0);

     // save the index
    //make_filepath ("../tse-ouput/unittest-saved-index");
    char* old_save_to_dir = "./unittest-saved-index";
    index_save(index2,old_save_to_dir);
    EXPECT(count_net() != 0);
     
    
    // clean up index2
    index_delete(index2, item_index_delete); // clean up
    EXPECT(count_net() == 0);

     // load the index
    FILE *ofp = assertp(fopen(old_save_to_dir, "r"),"oldIndexFilename could not be opened for reading\n");
    EXPECT(ofp != NULL);
    index_t* index3 = index_new(2*(lines_in_file(ofp)+1));
    EXPECT(index3 != NULL);
    index3 = index_load(index3, ofp) ;
    EXPECT(index3 != NULL);
    //make_filepath("./tse-output/unittest-loaded-index");
    char* load_to = "./unittest-loaded-index";
    index_save(index3,load_to);
    EXPECT(count_net() != 0);
        
     // clean up
    index_delete(index3, item_index_delete); // clean up
    EXPECT(count_net() == 0);
     
    END_TEST_CASE;
    return TEST_RESULT;

}

////////////////////////////////////////////
/************************  main ********************************/
int
main(const int argc, const char *argv[])
{
  int failed = 0;

  failed += test_newindex0();
  failed += test_newindex1();
  failed += test_newindex2();

  if (failed) {
    printf("\nFAILED %d test cases", failed);
    return failed;
  } else {
    printf("\nPASSED all test cases\n");
    return 0;
  }
}

/********************** static helper functions implemetations *************************/

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
    //free(item);   //nothing to free since items were not malloc space allocated
  }

}   

// /**************** item_index_delete ****************/
 /* Frees item memory of items in index ie. counterset.
  */
 static void item_index_delete(void *item)
 {
     if (item != NULL) {
        counters_delete(item); // delete the counterset
        item = NULL;
     }
 }

#endif // UNIT_TEST

