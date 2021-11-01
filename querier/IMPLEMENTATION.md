# Salifyanji J Namwila
## CS50 Fall 2021, Lab 6 Querier Implementation

### Functions/modules

`querier` is structured as follows:

`querier` reads the documents in a given  crawler-produced `directory` and the index  file produced by indexer, to interactively process written queries entered  onto the console by the user.

``` c

// visible outside the module
int check_query_input(int argc, char *argv[]);
void process_query(index_t *idx, char * directory);
bool is_satify_query(const char * arr[], int count);
void run_query(index_t *idx, char * directory, const char* arr[], int size);
```
```c
// not visible outside the module

1. static void prompt(void);
2. static void counters_merge(counters_t *ctrs_A, counters_t *ctrs_B);
```
```c
3. static void counters_merge_helper(void *arg, const int key, int count_B);
4. static void counters_intersect(counters_t *ctrs_A, counters_t *ctrs_B);
```
```c
5. static void counters_intersect_helper(void *arg, const int key, int count_A);
6. static char *getDocumentURL(char * directory, int docID);
7. static bool isOperator (const char *word);
```
```c
8. static counters_t* counters_product (index_t *idx, const char * arr[], int curPos, int * posAddress, int arr_size);
```
```c
9. static counters_t* counters_sum (index_t *idx, const char * arr[], int size);
```
```c
10. static void count_matches(void *arg, const int key, int count);
11. static void sort_matches(void *arg, const int docID, int myScore);
12. static void item_delete(void *item);
13. static void copy(void *arg, const int key, const int count);

```


`querier` uses following helpfer functions from the `common` library:
  1. *indexbuild*, which builds the index from `crawler` produced documents.
  2. *normalize_word*, which converts a query to lowercase
  3. *isCrawlerDirectory*, which determines if `pageDirectory` was produced by `crawler`.

### Major data structures

 1. `hashtable` (as the index) 
 2. `set` (built into hashtable)
 3. `countersets` (of docID docID, docID score, and docID count pairs)
 4. `word` has function `get_tokens` and `isAlpha` to validate the query.


### Pseudo code for logic/algorithmic flow

### Pseudo code for querier

The `querier.c` implementation runs as follows (using C):

1. Execute from a command line with usage syntax `./querier pageDirectory indexFilename`
2. Validate its command-line arguments:
    1. Ensure `pageDirectory` is the pathname for an existing Crawler-generated directory by checking for the existence of .crawler
    2. Ensure `indexFilename` is the pathname of a readable, existing file
2. Load the index from `indexFilename` into an internal data structure.
3. Read search queries from stdin, one per line, until EOF.
	1. clean each query by changing the query to its lower case variation
    2. if the query syntax is somehow invalid, print an error message, do not perform the query, and prompt for the next query.
	3. print the 'clean' query for user to see.
	4. use the index to identify the set of documents that satisfy the query
        1. Check the next word in the query (if any)
            1. If the word is an OR, combine the current query into the final counters set
            2. Otherwise, add the current query to the temporary counters set of AND sequences
        2. If there are no more words and the temporary counters set is not empty, merge the temporary counters set into the final set by summing over the values
	5. if the query is empty (no words), print nothing.
	6. if no documents satisfy the query, print `No documents match.`
4. Rank the resulting set of documents according to its score, and print the set of documents in decreasing rank order; for each, list the score, document ID and URL.
    1. Obtain the URL by reading the first line of the relevant document file from the `pageDirectory`.
5. Exit with zero status when EOF is reached on stdin.

### Testing Plan

see `DESIGN.md` for more testing specific examples


### Limitations


