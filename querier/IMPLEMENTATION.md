# Sydney Friedland
## CS50 Spring 2020, Lab 6 Querier Implementation

### Functions/modules

`querier` is structured as follows:





`querier` uses following helpfer functions from the `common` library:
  1. *indexbuild*, which builds the index from `crawler` produced documents.
  2. *normalize_word*, which converts a query to lowercase
  3. *isCrawlerDirectory*, which determines if `pageDirectory` was produced by `crawler`.

### Major data structures

 1. *hashtable* (as the index) 
 2. *set* (built into hashtable)
 3. *countersets* (of docID docID, docID score, and docID count pairs)
 4. *array* (of result_t structs containing docID score pairs; a count component keeps track of the number of structs in the array)

### Pseudo code for logic/algorithmic flow


### Limits

