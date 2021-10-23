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

/**************** global types ****************/
typedef hashtable_t index_t; // opaque to users of the module

/**************** functions ****************/

/**************** index_new ****************/
/* Create a new (empty) index.
 *
 * Caller provides:
 *   number of slots to be used for the hashtable (must be > 0).
 * We return:
 *   pointer to the new index; return NULL if error.
 * We guarantee:
 *   index is initialized empty.
 * Caller is responsible for:
 *   later calling index_delete.
 */
index_t*
index_new(const int num_slots);

/**************** index_insert ****************/
/* Insert item, identified by key (string), into the given index.
 *
 * Caller provides:
 *   valid pointer to index, valid string for key, valid pointer for item.
 * We return:
 *   false if key exists in ht, any parameter is NULL, or error;
 *   true iff new item was inserted.
 * Notes:
 *   The key string is copied for use by the index; that is, the module
 *   is responsible for allocating memory for a copy of the key string, and
 *   later deallocating that memory; thus, the caller is free to re-use or 
 *   deallocate its key string after this call.  
 */
bool
index_insert(index_t* index, const char *key, void *item);

/**************** index_find ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   valid pointer to index, valid string for key.
 * We return:
 *   pointer to the item corresponding to the given key, if found;
 *   NULL if index is NULL, key is NULL, or key is not found.
 * Notes:
 *   the index is unchanged by this operation.
 */
void*
index_find(index_t* index, const char* key);

/**************** indexe_iterate ****************/
/* Iterate over all items in the table; in undefined order.
 *
 * Caller provides:
 *   valid pointer to index, 
 *   arbitrary void*arg pointer,
 *   itemfunc that can handle a single (key, item) pair.
 * We do:
 *   nothing, if ht==NULL or itemfunc==NULL.
 *   otherwise, call the itemfunc once for each item, with (arg, key, item).
 * Notes:
 *   the order in which index items are handled is undefined.
 *   the index and its contents are not changed by this function,
 *   but the itemfunc may change the contents of the item.
 */
void
index_iterate(index_t* index,  void *arg, void (*itemfunc)(void *arg, const char *key, void *item));

/**************** index_print ****************/
/* Print the whole table; provide the output file and func to print each item.
 * 
 * Caller provides:
 *   valid pointer to index, 
 *   FILE open for writing,
 *   itemprint that can print a single (key, item) pair.
 * We print:
 *   nothing, if NULL fp.
 *   "(null)" if NULL ht.
 *   one line per hash slot, with no items, if NULL itemprint.
 *   otherwise, one line per hash slot, listing (key,item) pairs in that slot.
 * Note:
 *   the index and its contents are not changed by this function,
 */

// void index_print(index_t *index, void *fp, void (*itemprint)(void *fp, const char *key, void *item));

/**************** index_delete ****************/
/* Delete index, calling a delete function on each item.
 *
 * Caller provides:
 *   valid index pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if index==NULL, do nothing.
 *   otherwise, unless itemfunc==NULL, call the itemfunc on each item.
 *   free all the key strings, and the index itself.
 * Notes:
 *   We free the strings that represent key for each item, because 
 *   this module allocated that memory in index_insert.
 */

/**************** index_delete ****************/
/* Delete index, calling a delete function on each item.
 *
 * Caller provides:
 *   valid index pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if index==NULL, do nothing.
 *   otherwise, unless itemfunc==NULL, call the itemfunc on each item.
 *   free all the key strings, and the index itself.
 * Notes:
 *   We free the strings that represent key for each item, because 
 *   this module allocated that memory in index_insert.
 */
void
index_delete(index_t *ht, void (*itemdelete)(void *item) );


/******************************* index_build() **********************************/
/* Builds an inverted-index data structure mapping from words to (docID, count)
 * pairs, wherein each count represents the number of occurrences of the given
 * word in the given document. The function receives an empty index index
 * as a parameter, then populates and returns a the index. Words with
 * fewer than three characters are ignored and are "normalized" before they
 * are indexed. Here, "normalized" means all letters are converted to lower-case. 
 */ 
index_t*
index_build(index_t *ht, char *dir);

/******************************* index_save() ************************************/
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
void
index_save(index_t *ht, char *filename);

/********************************* index_load() **********************************/
/* Loads the index from the user provided oldIndexFilename
 * into an inverted-index data structure and saves that 
 * structure in newIndexFilename. The function receives
 * a index_t type as a parameter then returns the 
 * populated index.
 */ 
index_t*
index_load(index_t *ht, FILE *fp);

#endif // __INDEX_H
