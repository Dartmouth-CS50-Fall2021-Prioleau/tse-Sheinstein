
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



/********************** file-local global variables **********************/
/* none */

/******************************* local types ****************************/
/************ two_ctrs_t ****************/
typedef struct two_ctrs {
  counters_t *c1;
  counters_t *c2;
} two_ctrs_t;

/************* document_t ***************/
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

/************************ global functions **********************/
/* that is, visible outside this file */

/*********************** local functions *************************/
/* not visible outside this file */
static void prompt(void);
static void counters_merge(counters_t *ctrs_A, counters_t *ctrs_B);
static void counters_merge_helper(void *arg, const int key, int count_B);
static void counters_intersect(counters_t *ctrs_A, counters_t *ctrs_B);
static void counters_intersect_helper(void *arg, const int key, int count_A);
static char *getDocumentURL(char * directory, int docID);
static bool isOperator (const char *word);
static void count_matches(void *arg, const int key, int count);
static void sort_matches(void *arg, const int docID, int myScore);
static void item_delete(void *item);


/***************************** Function Prototypes **************************/
int check_input(int argc, char *argv[]);
void process_query(index_t *idx, char * directory);
bool is_satify_query(const char * arr[], int count);
void run_query(index_t *idx, char * directory, const char* arr[], int size);
counters_t* counters_product (index_t *idx, const char * arr[], int curPos, 
int * posAddress, int arr_size);
counters_t* sum_counters (index_t *idx, const char * arr[], int size);
void copy(void *arg, const int key, const int count);


/***************************** main() ****************************************/
int
main(int argc, char *argv[]) 
{
    // validate parameters
    int error_code;
    if ( (error_code = check_input(argc, argv)) != 0){
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


/**************************** helper function implementation **************************/

/*************************************** check_input() *********************************/
/* Makes sure input directory exist and is indeed cra;wer produced
* exits non-zero if the number of inputs !=3, directory is not crawler prodeuced,
or index filename is not readable.
*/
int
check_input(int argc, char *argv[])
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


/****************************** process_query() *********************************/
/* 
* Processes each query, and prompts the next one
*/


void process_query(index_t *idx, char * directory)
{
    // max number of words we expect on a single query
    int NUMSLOTS = 50; // create index 
    const char * extracted_words[NUMSLOTS]; // allocate memory for query tokens on stack

    prompt(); // request query if a tty

    char* line; 
    while((line = freadlinep(stdin)) != NULL){
     
        int count  = get_tokens(line, extracted_words);

        if((count > 0) && (is_satify_query(extracted_words, count)))
        {    
            printf("query: ");
            for (int i = 0; i < count; i ++){
                printf(" %s", extracted_words[i]);
            }
            printf("\n");

            //run the query
            run_query(idx, directory, extracted_words, count); 
        }
        free(line);
        memset(extracted_words, '\0', sizeof(extracted_words));
        prompt();

    }

}



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



/****************************** is_satify_query() ****************************/
/* 
* Checks for invalid query.
 Returns true if query is valid, 
 Returns false if arr of word toekns is NULL ,or a space token was extracted,
 or an empty token was extracted.
 *Also retruns false if an operator "and " or "or" occurs at the beginning or end 
 of query 
 *And finally, also returns false if two operators are adjacent to
 in a query.
 running_product_copywise returns false
*/
bool
is_satify_query(const char * arr[], int count)
  {
    // check for empty query
    if (arr[0] == NULL || strcmp(arr[0], " ") == 0 || strcmp(arr[0], "") == 0){
         return false;
    }

    //check for operator at beginning or end
    if ( isOperator(arr[0]) || isOperator(arr[count-1]) ){
        printf("Error: the words: \"and\" \"or\" cannot be first or last\n");
        return false;
    }

    //check for consecutive operators
    for (int i = 0; i < count -2; i ++){
        if (isOperator(arr[i]) && isOperator(arr[i+1])){
            printf("Error: cannot have two consecutive operators\n");
            return false;
        }
    }
    // Valid query
    return true;

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



/*******************************run_query() *****************************/
/*
* Searches for matches to the query tokens in given index of word:document pages,
* scores them and prints a list of documents in ranked order.
*/
void
run_query(index_t *idx, char * directory, const char* arr[], int size)
{
    counters_t *result = sum_counters(idx, arr, size);

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

/************************** sum_counters() ********************************/
/* 
* Computes a running sum of counters items
*/
counters_t*
sum_counters (index_t *idx, const char * arr[], int size) {
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
counters_t*
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


// make a copy of a counters struct
void copy(void *arg, const int key, const int count){
    if (arg != NULL){
        counters_t *running_product_copy = (counters_t *) arg;
        counters_set(running_product_copy, key, count);
    }
}


/*********************************** counters_intersect() *********************/
/* 
* Intersects two counters
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



/***************************** counters_intersect_helper() **********************/ 
/* 
* Helper for counters_intersect
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
        i ++;
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


