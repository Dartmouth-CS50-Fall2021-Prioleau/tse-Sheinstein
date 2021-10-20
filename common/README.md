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

hashtable_t
index_biuld(hashtable_t *ht, char* directory);

void
index_save(hashtable_t *ht, char* filename);

hashtable_t*
index_load(hashtable *ht, FIL* fp);
```

### Compilation 

To build the library, simply `make`, which builds a common archive `common.a`.

To clean up any binary/object files produced by compiler, backup files, core dumps etc, call `make clean`.