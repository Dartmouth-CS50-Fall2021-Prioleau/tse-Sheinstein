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
/* none */

/*************************** global types *******************************/
typedef struct result { 
    int docID;
    int score; 
    int count;
} result_t;

/************************ global functions **********************/
/* that is, visible outside this file */

/*********************** local functions *************************/
/* not visible outside this file */
static void prompt(void);

/************************ function prototypes ********************/
int fileno(FILE *stream);
bool isOnlyAlphabet(char *word);
bool isValidAndOrSyntax(char words[][50]);
void query(char* line, hashtable_t *ht, char *dir);
void querier(char words[][50], hashtable_t *ht, char *dir);
void get_ctrskey(void *arg, const int key, const int count);
void get_ctrssize(void *arg, const int key, const int count);
void initialize_result(void *arg, const int key, const int count);
void sort_result(result_t *results);
void print_query(result_t *results, char *dir);
void print_URL(int docID, char *dir);
void item_delete(void *item);

/***************************** main() *****************************/
int
main(int argc, char* argv[]) {

    // validate parameters
    if (argc != 3) {
        fprintf (stderr, "incorrect number of parameters\n"); // check number of params
        return 1;
    }
    
    // check if pageDirectory is the pathname for an existing directory produced by the crawler
    if (!isCrawlerDirectory(argv[1])) return 1; // error printed from pagedir

    // check if indexFilename is the pathname of a readable file
    FILE *fp = assertp(fopen(argv[2], "r"), "indexFilename could not be opened for reading\n"); 
    
    // load the inverted index
    const int NUMSLOTS = 2*(lines_in_file(fp)+1); // create index 
    index_t* index = index_new(NUMSLOTS);
    if (index == NULL) {
        fprintf(stderr, "index_new failed\n");
        fclose(fp);
        return 1;
    }

    printf("Loading index...\n");
    index = index_load(index, fp); // load index data structure
    fclose(fp);
    
    prompt(); // request query if a tty
    char *line;
    while ((line = freadlinep(stdin)) != NULL) {
        query(line, index, argv[1]); // reads a query and does the querying
        free(line);
        prompt(); // request next query if a tty
    }

     // clean up
    printf("Cleaning up...\n");
    index_delete(index, item_delete);

    // report task completion
    printf("Done.\n");

    return 0;
}



/**************************** helper function implementation **************************/

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



/************************** query() ***************************/
/* Reads a query from stdin until EOF detected
 * and calls other functions to perform the querying.
 */
void
query(char* line, index_t *index, char *dir) 
{
    char *linecopy = (char *)malloc((strlen(line) + 1)*sizeof(char)); // make copy for formatting purposes
    if (linecopy != NULL) {
        line = normalize_word(line); // make lowercase
        strcpy(linecopy, line);

        char words[50][50] = {'\0'}; // assume no more than 50 words, 50 characters each, in query
        char *word = strtok(line, " "); // extract words from query, using " " as the delimiter


        
        int i = 0;
        bool querycheck = false; // used to determine if program should proceed with querier

        while (word != NULL) {
            if ((querycheck = isOnlyAlphabet(word)) == true) { // check if word is alphabet
                strcpy(words[i], word); // add to array
                word = strtok(NULL, " "); // get next word
                i++;
            }
            else break;
        }
        
        if ((querycheck == true) && (isValidAndOrSyntax(words) == true)) { // any errors printed from isValidAndOrSyntax()
            printf("query: %s\n", linecopy);
            querier(words, index, dir); // do the querying
        }

        free(linecopy);
        memset(words, '\0', sizeof(words)); // empty array for next query
    }
    else fprintf(stderr, "could not allocate memory for linecopy\n");
}















/***************************** isOnlyAlphabet() *********************************/
/* Checks if a word consists only of alphabet
 * characters. Returns true if that is the case,
 * otherwise prints an error and returns false.
 */
bool
isOnlyAlphabet(char *word) 
{
    int i;
    for (i = 0; i < strlen(word); i++) {
        if (!isalpha(word[i])) { // check for bad characters
            fprintf(stderr, "Error: bad character '%c' in query.\n", word[i]);
            return false; 
        }
    }
    return true;
}



/******************************* isValidAndOrSyntax *****************************/
/* Checks for the following syntax errors and 
 * returns true if no error, false if error. If
 * returning false, function will print an error
 * message.
 *  'and' 'and' adjacent
 *  'and' 'or' or 'or' 'and' adjacent
 *  'or' 'or' adjacent
 *  first word 'and' or 'or'
 *  last word 'and' or 'or'
 */
bool
isValidAndOrSyntax(char words[][50])
{
    char *and = "and";
    char *or = "or";
    if ((strcmp(words[0], and) == 0)) { // check if 'and' is first word
        fprintf(stderr, "Error: 'and' cannot be first\n");
        return false; 
    }
    else if ((strcmp(words[0], or) == 0)) { // check if 'or' is first word
        fprintf(stderr, "Error: 'or' cannot be first\n");
        return false; 
    }
    else {
        int i = 1;
        while (strlen(words[i]) > 0) { // check all adjacent cases
            if ((strcmp(words[i-1], and) == 0) && (strcmp(words[i], or) == 0)) {
                fprintf(stderr, "Error: 'and' and 'or' cannot be adjacent\n");
                return false;
            }
            else if ((strcmp(words[i-1], or) == 0) && (strcmp(words[i], and) == 0)) {
                fprintf(stderr, "Error: 'or' and 'and' cannot be adjacent\n");
                return false;
            }
            else if ((strcmp(words[i-1], or) == 0) && (strcmp(words[i], or) == 0)) {
                fprintf(stderr, "Error: 'or' and 'or' cannot be adjacent\n");
                return false;
            }
            else if ((strcmp(words[i-1], and) == 0) && (strcmp(words[i], and) == 0)) {
                fprintf(stderr, "Error: 'and' and 'and' cannot be adjacent\n");
                return false;
            }
            i++;
        }
        if (strcmp(words[i-1], and) == 0) { // check last word in query
            fprintf(stderr, "Error: 'and' cannot be last\n");
            return false;
        }
        else if (strcmp(words[i-1], or) == 0) { // check last word in query
            fprintf(stderr, "Error: 'or' cannot be last\n");
            return false;
        }
        return true; // if query passes all cases
    }
}



/*************************************** querier() *****************************/
/* Does the querying */
void
querier(char words[][50], index_t *index, char *dir) 
{   
    counters_t *qryIDs = counters_new(); // to hold docIDs found (corresponding to query) 
    int i = 0;
    if (qryIDs != NULL) {
        while (strlen(words[i]) > 0) { // for each word read from stdin 
            counters_t *ctrs = index_find(index, words[i]);
            if (ctrs != NULL) { // if word is stored in index                
                counters_t *IDs = counters_new();
                if (IDs != NULL) {
                    counters_iterate(ctrs, (void*)IDs, get_ctrskey); // get all docIDs for a given word, put into IDs
                    int j, ID;
                    for (j = 0; j < 500; j++) { // assume no more than 500 documents
                        if ((ID = counters_get(IDs, j)) != 0 ) { // for each docID found for a given word
                            int score;
                            if ((score = counters_get(qryIDs, ID)) != 0) { // if docID already found for a different word
                                int addscore = counters_get(ctrs, ID);
                                counters_set(qryIDs, ID, score + addscore); // increment score
                            }
                            else { // if first occurrence of docID
                                score = counters_get(ctrs, ID); // get score
                                counters_set(qryIDs, ID, score); // add to qryIDs
                            }
                        }
                    }
                    counters_delete(IDs);
                } else fprintf(stderr, "counters_new failed\n");
            }
            i++;
        }
    }
    int size = 0;
    counters_iterate(qryIDs, (void*)&size, get_ctrssize); // determine how many docID score pairs exist in qryIDs

    result_t results[size]; // create array of result_t with correct size
    results[0].count = size;
    for (i = 0; i < size; i++) { // intialize all docIDs to 0
        results[i].docID = 0;
    }
    counters_iterate(qryIDs, (void*)results, initialize_result); // transfer info from qryIDs to results
    sort_result(results);
    print_query(results, dir); // print formatted query results to stdout 
    free(qryIDs);
}



/************************** get_ctrskey() *******************************/
/* Sets both the key and value of a counterset, 
 * passed in as arg, to the docID of the counterset
 * being iterated upon.
 */
void
get_ctrskey(void *arg, const int key, const int count)
{
    counters_set(arg, key, key);
}



/************************* get_ctrssize() ******************************/
/* Increments arg to reflect the number of docID 
 * score pairs in a counterset. 
 */
void
get_ctrssize(void *arg, const int key, const int count)
{
    *((int *)arg) = *((int *)arg) + 1;
}



/*************************** initialize_result() **************************/
/* Initializes result_t struct with docID score pairs
 * from a counterset. */
void
initialize_result(void *arg, const int key, const int count)
{   
    int i = 0;
    while (((result_t*)arg)[i].docID != 0) { // find next "empty" struct element
        i++;
    }
    // only insert docID score pairs into struct elements that do not contain docID score pairs
    ((result_t*)arg)[i].docID = key;
    ((result_t*)arg)[i].score = count;
}



/******************************* sort_result() ****************************/
/* Sorts query results in descending order by 
 * score. */
void
sort_result(result_t *results)
{   
    int i, j, swap_score, swap_docID;
    for(i = 0; i < results[0].count; i++) {
        for(j = (i+1); j < results[0].count; j++) {
            if(results[i].score < results[j].score) {
                swap_score = results[i].score;
                swap_docID = results[i].docID;
                results[i].score = results[j].score;
                results[i].docID = results[j].docID;
                results[j].score = swap_score;
                results[j].docID = swap_docID;
            }
        }
    }
}



/****************************** print_query() *****************************/
/* Prints a query. Prints the number of documents
 * with matches, score for each doc, docID, 
 * and docID url. */
void
print_query(result_t *results, char *dir)
{
    if (results[0].count == 0) printf("No documents match.");
    else if (results[0].count == 1) { // formatting, print singular, document
        printf("Matches %d document:", results[0].count);
        printf("\nScore %3d doc %3d: ", results[0].score, results[0].docID);
        print_URL(results[0].docID, dir); // get URl for docID from pageDirectory
    }
    else { // formatting, print plural, documents 
        printf("Matches %d documents:", results[0].count);
        int i;
        for (i = 0; i < results[0].count; i++) {
            printf("\nScore %3d doc %3d: ", results[i].score, results[i].docID);
            print_URL(results[i].docID, dir); // get URl for docID from pageDirectory
        }
    }
    printf("\n--------------------------------------------------------\n");
}



/********************************** print_URL () *******************************/
/* Prints the URL of a given docID. Function
 * aquires URL pageDirectory, assumes URl is
 * first line of a document.
 */
void
print_URL(int docID, char *dir)
{
    int length = strlen(dir);
    char path[length+10]; // create string with space for dir and docID
    sprintf(path, "%s/%d", dir, docID); // create path name

    FILE *fp = fopen(path, "r"); // attempt to open file for reading
    if (fp != NULL) {
        char *URL = freadlinep(fp);
        printf("%s", URL); // print URL associated with docID
        free(URL);
        fclose(fp);
    }
    else fprintf(stderr, "error opening file\n");
}



/******************************** item_delete() *****************************/
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

