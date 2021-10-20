/**
 * index.h -- header file for CS50 'index' module
 * 
 * 'index' module contains methods that need to be shared by crawler, 
 * indexer, and querier. More specifically, 'index' helps the indexer 
 * read the documents in a given `directory` outputted by the crawler, 
 * builds an inverted index mapping from words to document ID and count, 
 * and writes that index to a file
 * 
 * Salifyanji J Namwila
 * October 19 2021
 */

#ifndef __INDEX_H
#define __INDEX_H


#include <stdio.h>
#include "../libcs50/hashtable.h"

/**************** index_build ****************/
/* Builds an inverted-index data structure mapping from words to (docID, count)
 * pairs, wherein each count represents the number of occurrences of the given
 * word in the given document. The function receives an empty index hashtable
 * as a parameter, then populates and returns a the hashtable. Words with
 * fewer than three characters are ignored and are "normalized" before they
 * are indexed. Here, "normalized" means all letters are converted to lower-case. 
 */ 
hashtable_t* index_build(hashtable_t *ht, char *dir);

/**************** index_save ****************/
/* Creates a file using the user provided indexFilename 
 * and writes the index to that file in the following format:
 * 
 * one line per word, one word per line
 * each line provides the word and one or more (docID, count) pairs, in the format:
 *      word docID count [docID count]…
 * where word is a string of lower-case letters,
 * where docID is a positive non-zero integer,
 * where count is a positive non-zero integer,
 * where the word and integers are separated by spaces.
 * 
 * Within the file, the lines may be in any order.
 * Within a line, the docIDs may be in any order. 
 */
void index_save(hashtable_t *ht, char *filename);

/**************** index_load ****************/
/* Loads the index from the user provided oldIndexFilename
 * into an inverted-index data structure and saves that 
 * structure in newIndexFilename. The function receives
 * a hashtable_t type as a parameter then returns the 
 * populated hashtable.
 */ 
hashtable_t* index_load(hashtable_t *ht, FILE *fp);

#endif // __INDEX_H
