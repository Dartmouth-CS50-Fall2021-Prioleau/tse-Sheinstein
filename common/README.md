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
### Compilation

To compile, simply `make` to output `common.o`.