
/* 
 * querier.c - CS50
 *
 * see README.md for more information.
 *
 * Salifyanji J Namwila Oct 25 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include "../libcs50/bag.h"
#include "../common/word.h"
#include "../common/index.h"
#include "../common/pagedir.h"
#include "../libcs50/memory.h"
#include "../libcs50/file.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "querier.h"
#include "qunittest.h" // for unnitesting




/********************** file-local global variables **********************/
/* none */

/******************************* local types ****************************/
/************ two_ctrs_t ****************/
/* hold two counter sets
 * Later passed to counters_intersect
 to obtain the inntersention of the two
 counter sets
 *
 */

typedef struct two_ctrs {
  counters_t *c1;
  counters_t *c2;
} two_ctrs_t;

/************* document_t ***************/
/*structure that holds details of the document ot be
 * printed to stdout after query
 *
 */
typedef struct document {
  int score;
  int ID;
  char * URL;
} document_t;

/*********** arr_and_string_t ************/
typedef struct arr_and_string {
  document_t ** arr;
  int length;
  char * string;
} arr_and_string_t;


/*********************** local functions prototypes *************************/
/* not visible outside this file */
static void prompt(void);
static void counters_merge(counters_t *ctrs_A, counters_t *ctrs_B);
static void counters_merge_helper(void *arg, const int key, int count_B);
static void counters_intersect(counters_t *ctrs_A, counters_t *ctrs_B);
static void counters_intersect_helper(void *arg, const int key, int count_A);
static char *getDocumentURL(char * directory, int docID);
static bool isOperator (const char *word);
static counters_t* counters_product (index_t *idx, const char * arr[], int curPos, int * posAddress, int arr_size);
static counters_t* counters_sum (index_t *idx, const char * arr[], int size);
static void count_matches(void *arg, const int key, int count);
static void sort_matches(void *arg, const int docID, int myScore);
static void copy(void *arg, const int key, const int count);
static void item_delete(void *item);

/***************************** globa Function Prototypes **************************/
/* that is, visible outside this file */

int check_query_input(int argc, char *argv[]);
void process_query(index_t *idx, char * directory);
bool is_satify_query(const char * arr[], int count);
void run_query(index_t *idx, char * directory, const char* arr[], int size);


/***************************** unittest helper function prototypes ****************/
static void itemcount(void* arg, const int key, const int count);
int test_isOperator0();
int test_newindex0();
int test_countersSum0();
int test_copy();
int test_counters_merge0();
int test_counters_product0();


// conditional main methods
#ifdef UNIT_TEST



////////////////////////////////////////////
/************************  unnitest main method ********************************/
int
main(const int argc, const char *argv[])
{
  int failed = 0;

  failed += test_isOperator0();
  failed += test_newindex0();
  failed += test_countersSum0();
  failed += test_copy();
  failed += test_counters_merge0();
  failed += test_counters_product0();
  

  if (failed) {
    printf("\nFAILED %d test cases", failed);
    return failed;
  } else {
    printf("\nPASSED all test cases\n");
    return 0;
  }

}


#else



/***************************** main() ****************************************/
int
main(int argc, char *argv[]) 
{
    // validate parameters
    int error_code;
    if ( (error_code = check_query_input(argc, argv)) != 0){
        return error_code;
    }
    
    // check if indexFilename is the pathname of a readable file
    FILE *fp = assertp(fopen(argv[2], "r"), "indexFilename could not be opened for reading\n"); 
    //int file_size = lines_in_file(fp);

    // load the inverted index
    const int NUMSLOTS = 2*(lines_in_file(fp)+1); // create index 
    index_t* index = index_new(NUMSLOTS);

    if (index == NULL) {
        fprintf(stderr, "index_new failed\n");
        fclose(fp);
        return 1;
    }
    
    // load index from file
    printf("Loading index...\n");
    index = assertp(index_load(index, fp), "Error loading index from file");
    fclose(fp);


    //prompt();
    // process Query from stdin
    process_query(index, argv[1]);

    // clean up
    printf("Cleaning up...\n");
    index_delete(index,item_delete);

    // report task completion
    printf("Done.\n");

    return 0;
}

#endif // UNIT_TEST





/****************************  global  helper  functions implementation **************************/

/*************************************** check_query_input() *********************************/
/*
 * see querier.h for description
 *
*/
int
check_query_input(int argc, char *argv[])
{
    //Check for correct number of parameters
    if (argc != 3){
        fprintf(stderr, "Usage: ./querier pageDirectory indexFilename\n");
        return 1;
    }
    //Validate directory name
    if ( ! isCrawlerDirectory(argv[1])){
        fprintf(stderr, "pageDirectory is not an existing directory produced by the crawler\n");
        return 2;
    }
    //Validate file name
    FILE *fp; 
    fp = fopen(argv[2], "r");
    if (fp == NULL){
        fprintf(stderr, "indexFilename is not a readable file\n");
        return 3;
    }
    fclose(fp);
    return 0;
}



/****************************** is_satify_query() ****************************/
/*
 * see querier.h for description
 *
*/
bool
is_satify_query(const char * arr[], int count)
  {
    // check for empty query
    if (arr[0] == NULL || strcmp(arr[0], " ") == 0 || strcmp(arr[0], "") == 0){
         return false;
    }

    //check for operator at beginning or end
    if ( isOperator(arr[0]) ){
        printf("Error: the word %s cannot be first \n", arr[0]);
        //fprintf(stdout, "-----------------------------------------------------------------------------------\n");
        return false;
    }
    if ( isOperator(arr[count-1]) ){
        printf("Error: the word %s cannot be last \n", arr[count-1]);
        //fprintf(stdout, "-----------------------------------------------------------------------------------\n");
        return false;
    }


    //check for consecutive operators
    for (int i = 0; i < count -2; i ++){
        if (isOperator(arr[i]) && isOperator(arr[i+1])){
            printf("Error: cannot have two consecutive operators\n");
            //fprintf(stdout, "-----------------------------------------------------------------------------------\n");
            return false;
        }
    }
    // Valid query
    return true;

}




/****************************** process_query() *********************************/
/*
 * see querier.h for description
 *
*/


void process_query(index_t *idx, char * directory)
{
    // max number of words we expect on a single query
    int NUMSLOTS = 50; // create index 
    const char * extracted_words[NUMSLOTS]; // allocate memory for query tokens on stack

    prompt(); // request query if a tty

    char* line; 
    //bool success = false; //determines if  query should run

    while((line = freadlinep(stdin)) != NULL){
        int num_words;
        bool words_status  = is_alpha_tokens(line, extracted_words, &num_words); //determines if  query should run

        if((words_status == true) && (num_words > 0) && (is_satify_query(extracted_words, num_words)))
        {    
            printf("query: ");
            for (int i = 0; i < num_words; i ++){
                printf(" %s", extracted_words[i]);
            }
            printf("\n");

            //run the query
            run_query(idx, directory, extracted_words, num_words); 
        }
        else{
            fprintf(stdout, "-----------------------------------------------------------------------------------\n");
        }
        
        free(line);
        memset(extracted_words, '\0', sizeof(extracted_words));
        prompt();

    }

}




/*******************************run_query() *****************************/
/*
 * see querier.h for description
 *
*/
void
run_query(index_t *idx, char * directory, const char* arr[], int size)
{
    counters_t *result = counters_sum(idx, arr, size);

    // Count the number of matches
    int num_matches = 0;
    counters_iterate(result, &num_matches, count_matches);
    if (num_matches == 0){
        printf("no matches\n");
        fprintf(stdout, "-----------------------------------------------------------------------------------\n");
        return;
    }

    ///////////////////////////////////////////
    // rank the matches by score

    // create a new array to use as a ranked list
    document_t *ranked_list[num_matches];
    memset(ranked_list, '\0', sizeof(ranked_list));

    // encapsulate all information vital  for ranking into struct
    arr_and_string_t *arr_and_s = malloc (sizeof(arr_and_string_t));
    arr_and_s->string = directory; // directory useful for later extracting URL
    arr_and_s->arr = ranked_list; // list
    arr_and_s->length = num_matches; // number of matches
    

    //////////////////////////////////////////
    // rank the matches by score

    // Iterate over the counters, passing the packaged information
    counters_iterate(result, arr_and_s, sort_matches);

    /////////////////////////////////////
    // print the ranked matching documents formated
    printf("matches %3d documents (ranked):\n", num_matches);
    for (int i = 0; i < num_matches; i ++){
        document_t *doc = ranked_list[i];
        printf("score: %3d, docID: %3d : %s\n", doc->score, doc->ID, doc->URL);
    }
    // demacate stdout for clear formatting
    fprintf(stdout, "-----------------------------------------------------------------------------------\n");
    // clean up
    for (int i = 0; i < num_matches; i ++){
        document_t *doc = ranked_list[i];
        count_free(doc->URL);
        count_free(doc);
    }
    counters_delete(result);
    count_free(arr_and_s);
}



/*************************************** local helper functions ***********************************************/


/*************************** prompt() ****************************/
/* Function given by cs50 Lab 6 instructions. 
 * Prints a prompt only if stdin is a tty (terminal).
 */
static void prompt(void)
{
    if (isatty(fileno(stdin))) {
        printf("Query? ");
    }
}



/************************** isOperator() ***************************/
/* 
* Returns true if word is "and" or "or"
*/
static bool
isOperator (const char *word){
    if( (strcmp (word, "and") == 0) || (strcmp(word, "or") == 0) ){
        return true;
    }
    return false;
}




/************************** counters_sum() ********************************/
/* 
* Computes a running sum of counters items
*/
static counters_t*
counters_sum (index_t *idx, const char * arr[], int size) {
    counters_t *running_sum = counters_new();
    int pos = 0;
    while (pos < size){
        counters_t *product = counters_product(idx, arr, pos, &pos, size);
        counters_merge(running_sum, product);
        counters_delete(product);

    }
    
    return running_sum;
}




/***************************** counters_product() ****************************/
/* 
* Computes a product of counters items
*/
static counters_t*
counters_product (index_t *idx, const char * arr[], int pos, int * posAddress, int arr_size) {
    const char * word = arr[pos];
    counters_t *running_product = index_find(idx, word);
    counters_t* running_product_copy = counters_new();
    counters_iterate(running_product, running_product_copy, copy);
    for (int i = pos + 1; i < arr_size; i ++){
        word = arr[i];
        *posAddress = i;
        if (strcmp(word, "and") == 0){
            continue;
        }
        if (strcmp(word, "or") == 0){
            *posAddress += 1;
            return running_product_copy;
        }
        counters_intersect(running_product_copy, index_find(idx, word));
    }
    *posAddress += 1;
    return running_product_copy;
}



/******************** counters_merge() *************************************/
/* 
* Merges the second  counterset ctrs_B into the first counterset ctrs_A:
* The second counterset is unchanged
*/
static void 
counters_merge(counters_t *ctrs_A, counters_t *ctrs_B)
{ 
  
  counters_iterate(ctrs_B, ctrs_A, counters_merge_helper);
}



/*************************** counters_merge_helper() ***********************/
/* 
* Helper for counters_merge
* consider one item for insertion into the running_product_copy counterset
*/
static void 
counters_merge_helper(void *arg, const int key, int count_B)
{
  counters_t * ctrs_A = arg;
  
  // find the same key in setA and update the count accordingly
  int count_A = counters_get(ctrs_A, key);
  if (count_A == 0) {
    // not found: insert it
    counters_set(ctrs_A, key, count_B);
  } else {
    // add to the existing value
    counters_set(ctrs_A, key, count_A+count_B);
  }
}


/****************************** copy () **************************************/
/* make a copy of a counters struct
 *
 */
static void
copy(void *arg, const int key, const int count)
{
    if (arg != NULL)
    {
        counters_t *running_product_copy = (counters_t *) arg;
        counters_set(running_product_copy, key, count);
    }
}


/*********************************** counters_intersect() *********************/
/* 
 * Intersects two counters, where ctrs_A is the running product counters.
 *
*/
static void 
counters_intersect(counters_t *ctrs_A, counters_t *ctrs_B)
{   // make a copy of new counters ,modify that counter and nreturnh it as running total
    two_ctrs_t *two_ctrs = malloc(sizeof(two_ctrs_t));
    two_ctrs->c1 = ctrs_A;
    two_ctrs->c2 = ctrs_B;
    counters_iterate(ctrs_A, two_ctrs, counters_intersect_helper);
    count_free(two_ctrs);

}



/***************************** counters_intersect_helper() **************************/ 
/* 
 * Helper for counters_intersect
 *
*/
static void 
counters_intersect_helper(void *arg, const int key, int count_A)
{
    two_ctrs_t *two_ctrs = arg;

    int count_B = counters_get(two_ctrs->c2, key); 
    if (count_B == 0) {
        // not found this key in counters B, set the int to zero
        counters_set(two_ctrs->c1, key, 0);
    } else {
        // found the key, take the minimum
        int count_final = count_A < count_B ? count_A : count_B;
        counters_set(two_ctrs->c1, key, count_final);
    }
}



/******************************** count_matches() ***********************************/
/* 
 * counts the number of non-zero count key-count pairs in counter
 */
static void
count_matches(void *arg, const int key, int count)
{
  int *n_matches = arg;

  if (n_matches!= NULL && count != 0)
    (*n_matches)++;
}



/******************************* sort_matches() **************************************/
/* 
 * Inserts document matches into an array, sorted by score
 */
static void
sort_matches(void *arg, const int docID, int myScore)
{
    // Ignore docs with no matches (score 0)
    if (myScore == 0){
        return;
    }

    // Unpackage the what we need to work with
    arr_and_string_t *arr_and_s = arg;

    document_t **arr = arr_and_s->arr;
    int arr_length = arr_and_s ->length;
    char * directory = arr_and_s->string;

    //Get the document's URL using a function from pagedir.c
    char *URL = getDocumentURL(directory, docID);

    // Initialize Struct
    document_t *this_doc = malloc (sizeof(document_t));
    this_doc -> score = myScore;
    this_doc -> ID = docID;
    this_doc -> URL = URL;
    

    // Insert doc into correct place in the array. 
    // (Insertion Sort style)
    int i = 0;
    while (i < arr_length){
        document_t *doc = arr[i];
        if (doc == NULL) {
            arr[i] = this_doc;
            return;
        } 
        if (doc->score < myScore) {
            for (int j = arr_length - 2; j >= i; j --){
                arr[j + 1] = arr[j];
            }
            arr[i] = this_doc;
            return;
        } 
        i++;
    }
}




/******************************** item_delete() *****************************/
/* Frees item memory of items in index ie. counterset.
*/
static void
item_delete(void *item)
{
    if (item != NULL) {
        counters_delete(item); // delete the counterset
    }
}




/***************************** getDocumentURL() *******************************/
/* Returnss the URL of a document.
* User provies name of a crawler-produced directory 
* and document's ID.
* We return a pointer to URL of a webpage.
 Caller is resposible for later freeing pointer .
*/
static char
*getDocumentURL(char * directory, int docID)
{
    int buffer_size = strlen(directory) + 10 + 1 + 1; //Enough space for largest int
    char filename[buffer_size];
    sprintf(filename, "%s/%d", directory, docID);

    FILE *fp = assertp(fopen(filename, "r"), "Failed to open file in getDocumentURL");
    char* URL = freadlinep(fp);
    fclose(fp);
    return URL;
}




/************************ itemcount() ************************/
/* count the non-null items in the set.
 * note here we don't care what kind of item is in set.
 */
static void itemcount(void* arg, const int key, const int count)
{
  int* nitems = arg;

  if (nitems != NULL && key>=0 && count >=0){
    (*nitems)++;
    }
}


/*************************************************************************************
 ******************************** unit testing methods ***************************************
 ***********************************************************************************/

/* ==================== testing isOperator() =================*/
/////////////////////////////////////
// make test if word is "and" or "or" 
int test_isOperator0(){

     START_TEST_CASE("isOperator0");
     EXPECT(isOperator("you") == false);
     EXPECT(isOperator("and") == true);
     EXPECT(isOperator("AND") == false);
     EXPECT (isOperator("or") == true);
     EXPECT(isOperator("OR") == false);

    END_TEST_CASE;
    return TEST_RESULT;

}

/* ==================== testing index_new () =================*/
/////////////////////////////////////
// make test if word is "and" or "or" 
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



int test_copy()
{

    START_TEST_CASE("copy0");

    // make a counterset
    counters_t* counter1 = counters_new();
    EXPECT(counter1 != NULL);
    counters_add(counter1, 1);
    int count = 0;
    counters_iterate(counter1, &count, itemcount);
    EXPECT(count ==1);

    // add more keys nto counter1
    counters_add(counter1, 2);
    counters_add(counter1, 3);
    counters_add(counter1, 4);
    count = 0; 
    counters_iterate(counter1, &count, itemcount);
    EXPECT(count == 4);

    /// create new counter1 copy
    counters_t* counter1_copy = counters_new();
    EXPECT(counter1_copy != NULL);
    counters_iterate(counter1,counter1_copy, copy);
    count = 0;
    counters_iterate(counter1_copy, &count, itemcount);
    EXPECT(count == 4)

    // get some keys from counter1_copy, they should be identicasl to counter1
    EXPECT(counters_get(counter1_copy, 1) == 1);
    EXPECT(counters_get(counter1_copy, 2) == 1);
    EXPECT(counters_get(counter1_copy, 3) == 1);
    EXPECT(counters_get(counter1_copy, 4) == 1);
    
    // clean up 
    counters_delete(counter1);
    counters_delete(counter1_copy);
    //EXPECT(count_net() == 0);

    END_TEST_CASE;
    return TEST_RESULT;
  
}



int test_counters_merge0(){

    START_TEST_CASE("counters_merge0");


    // make a counterset A
    counters_t* counterA = counters_new();
    EXPECT(counterA != NULL);

    // add some keys to counter A
    counters_add(counterA, 5);
    counters_add(counterA, 6);

    // count keys in counter A 
    int count = 0;
    counters_iterate(counterA, &count, itemcount);
    EXPECT(count == 2);
    
     // make a counterset B
    counters_t* counterB = counters_new();
    EXPECT(counterB != NULL);

    // add some keys to counter B
    counters_add(counterB, 7);
    counters_add(counterB, 8);

    // count keys in counter B
    count = 0;
    counters_iterate(counterB, &count, itemcount);
    EXPECT(count == 2);

    // merge counterset B into counterset A
    counters_merge(counterA, counterB);

    // count keys in counter A fter merging it with B keys
    count = 0;
    counters_iterate(counterA, &count, itemcount);
    EXPECT( count == 4)

    //count B item (should remain unchanged)
    count = 0;
    counters_iterate(counterB, &count, itemcount);
    EXPECT(count == 2);
    
    // check that counter A jhas the right keys
    EXPECT(counters_get(counterA, 5) == 1);
    EXPECT(counters_get(counterA, 6) == 1);
    EXPECT(counters_get(counterA, 7) == 1);
    EXPECT(counters_get(counterA, 8) == 1);
    
    // clean up 
    counters_delete(counterA);
    counters_delete(counterB);

    END_TEST_CASE;
    return TEST_RESULT;

}




int test_counters_product0(){

    START_TEST_CASE("counters_product0");


    int L_NUMSLOTS = 6; // create index 
    const char * extracted_words[L_NUMSLOTS]; // allocate memory for query tokens on stack
    
    // add string words to array 
    extracted_words[0] = "page";
    extracted_words[1] = "and";
    extracted_words[2] = "playground";
    extracted_words[3] =  "and";
    extracted_words[4] = "algorithm";

    EXPECT(strcmp(extracted_words[0],"page") == 0);
    EXPECT(strcmp(extracted_words[1],"and") == 0);
    EXPECT(strcmp(extracted_words[2],"playground") == 0);
    EXPECT(strcmp(extracted_words[3], "and") == 0);
    EXPECT(strcmp(extracted_words[4],"algorithm") == 0);

    //////////////////////////////////////////
    index_t* index = index_new(10);
    EXPECT(index != NULL);
    
    
    // create 3 counters for the words
    counters_t* pages = counters_new();
    counters_add(pages, 1);

    counters_t* playground = counters_new();
    counters_add(playground, 1);

    counters_t* algorithm = counters_new();
    counters_add(algorithm, 1);

    // insert counters into index
    index_insert(index, "page", pages);
    index_insert(index, "playground", playground);
    index_insert(index, "algorithm", algorithm);
 
    // obtain running sum
    int pos = 0;
    counters_t* running_sum = counters_product(index, extracted_words, pos, &pos, 5);
    EXPECT(running_sum != NULL);
   
   // iterate over runnning sum
    int count = 0;
    counters_iterate(running_sum, &count, itemcount);
    EXPECT( count == 1)
    
    count_free(running_sum);
    

    index_delete(index,item_delete);
    memset(extracted_words, '\0', sizeof(extracted_words));
    END_TEST_CASE;
    return TEST_RESULT;

    

}









int test_countersSum0()
{
     
    START_TEST_CASE("counters_sum0");

    int L_NUMSLOTS = 6; // create index 
    const char * extracted_words[L_NUMSLOTS]; // allocate memory for query tokens on stack
    
    // add string words to array 
    extracted_words[0] = "page";
    extracted_words[1] = "or";
    extracted_words[2] = "playground";
    extracted_words[3] =  "or";
    extracted_words[4] = "algorithm";

    EXPECT(strcmp(extracted_words[0],"page") == 0);
    EXPECT(strcmp(extracted_words[1],"or") == 0);
    EXPECT(strcmp(extracted_words[2],"playground") == 0);
    EXPECT(strcmp(extracted_words[3], "or") == 0);
    EXPECT(strcmp(extracted_words[4],"algorithm") == 0);

    //////////////////////////////////////////
    index_t* index = index_new(10);
    EXPECT(index != NULL);
    
    
    // create 3 counters for the words
    counters_t* pages = counters_new();
    counters_add(pages, 1);

    counters_t* playground = counters_new();
    counters_add(playground, 1);

    counters_t* algorithm = counters_new();
    counters_add(algorithm, 1);

    // insert counters into index
    index_insert(index, "page", pages);
    index_insert(index, "playground", playground);
    index_insert(index, "algorithm", algorithm);


    counters_t* running_sum = counters_sum(index, extracted_words, 5);
    EXPECT(running_sum != NULL);


    index_delete(index,item_delete);
    memset(extracted_words, '\0', sizeof(extracted_words));
    END_TEST_CASE;
    return TEST_RESULT;

}


