# Salifyanji J Namwila October 12, 2021
## CS50 Fall 2021, Lab 4 -Crawler, `common`

### common 

The `common` directory assembles code that will eventually be shared by the crawler, indexer, and querier.

#### pagedir

*pagedir* is defined in `pagedir.h` and implemented in `pagedir.c`. The following functions are exported through `pagedir.h`:

```c

bool pagefetcher(webpage_t *page);
void pagesaver(int file_num, char *directory, webpage_t *page);
void pagescanner(webpage_t *page, hashtable_t *ht, bag_t *bag);
bool isCrawlerDirectory(char *dir);
```

### word
*word* is defined in `word.h` and implemented in `word.c`. The following functions are exported throuh `word.h`:

```c

char* normalize_word(char* word);
```

### index
*index* is defined in `index.h` and implemented in `index.c`.
### Compilation

To compile, simply `make` to output `common.o`. The following are exported through `index.h`:

```c
index_t* index_new(const int num_slots);
bool index_insert(index_t* index, const char *key, void *item);
void* index_find(index_t* index, const char* key);
void index_iterate(index_t* index,  void *arg, void (*itemfunc)(void *arg, const char *key, void *item));
void index_delete(index_t *ht, void (*itemdelete)(void *item) );
index_t* index_build(index_t *ht, char *dir);
void index_save(index_t *ht, char *filename);
index_t* index_load(index_t *ht, FILE *fp);
```

#### Function Desription

```c
1.
index_t* index_new(const int num_slots); Creates a new (empty) index.

2.
bool index_insert(index_t* index, const char *key, void *item);Inserts item, identified by key (string), into the given index.

3.
void* index_find(index_t* index, const char* key); Returns the item associated with the given key.

4.
void index_iterate(index_t* index,  void *arg, void (*itemfunc)(void *arg, const char *key, void *item)); Iterates over all items in the table; in undefined order.

5.
void index_delete(index_t *ht, void (*itemdelete)(void *item) ); Deletes index, calling a delete function on each item.
 
 6.
 index_t* index_build(index_t *ht, char *dir);
 Builds an inverted-index data structure mapping from words to (docID, count) pairs, wherein each count represents the number of occurrences of the given word in the given document. The function receives an empty index index as a parameter, then populates and returns a the index. Words with fewer than three characters are ignored and are "normalized" before they are indexed. Here, "normalized" means all letters are converted to lower-case. 

7.
void index_save(index_t *ht, char *filename);
Creates a file using the user provided indexFilename and writes the index to that file in the following format: one line per word, one word per line each line provides the word and one or more (docID, count) pairs, in the format: word docID count [docID count]…where word is a string of lower-case letters, where docID is a positive non-zero integer, where count is a positive non-zero integer, where the word and integers are separated by spaces. Within the file, the lines may be in any order. Within a line, the docIDs may be in any order. 

8.
index_t* index_load(index_t *ht, FILE *fp);
Loads the index from the user provided oldIndexFilename into an inverted-index data structure and saves that structure in newIndexFilename. The function receives  a index_t type as a parameter then returns the populated index.

```



#### Index.c Unittesting
`Unittest` IS DEFINED IN `unittest.h`. A glassbox of `index.c` unittesting is implemented .This way, index.c can run as a program if the unit test is defined. To run unittest user should simply run `make unittest` or `make unit` in the common directory. IMPORTANT:User should however ensure that they run crawler and have the `../tse-output/letters-depth-1` directory prodeced by crawler. This directory is is what is primarily used for testing the functionality of each one of the module's components. Failure to have the aforementioned driectory will lead to acute failure of the unit test program. Otherwise, the program effectively tests all the methods/ functions that index.c has to offer. To run valgrind on unittest, simply call `make valgrind`. The program test guarantees absolute zero memory leaks.

##### Running Index Unittest
The following are some importants remarks to runninng unnitest:
1. Make sure crawler is run first to produce  `../tse-output/letters-depth-1` directory on which parts of the  unittesting  is based.
2. To run the unittest, simply call `make unittest` or `make unit`. This call produces two files within the common directory namely `unittest-saved-index` and `unittest-loaded-index`. 
3. To rid off all object files and the two unittest files, simply run `make clean`.

### Compilation 

To build the library, simply `make`, which builds a common archive `common.a`.

To clean up any binary/object files produced by compiler, backup files, core dumps etc, call `make clean`.

