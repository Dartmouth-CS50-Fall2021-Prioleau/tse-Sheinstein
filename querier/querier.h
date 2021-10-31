/* 
 * querier.c - driver for 'querier' module
 *
 * The querier reads the documents in a given `directory` outputted by the 
 * crawler and the index file produced by indexer, to interactively answer 
 * written queries entered by the user
 * 
 * see IMPLEMENTATION and DESIGN spec for more information.
 *
 * Salifyanji J Namwila 
 * October 31, 2021
 * 
 */

#ifndef __QUERIER_H
#define __QUERIER_H

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



/*************************************** check_input() *********************************/
/* Makes sure input directory exist and is indeed cra;wer produced
* exits non-zero if the number of inputs !=3, directory is not crawler prodeuced,
or index filename is not readable.
*/
int
check_query_input(int argc, char *argv[]);



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
is_satify_query(const char * arr[], int count);



/****************************** process_query() *********************************/
/* 
* Processes each query, and prompts the next one
*/

void
process_query(index_t *idx, char * directory);


/*******************************run_query() *****************************/
/*
* Searches for matches to the query tokens in given index of word:document pages,
* scores them and prints a list of documents in ranked order.
*/
void
run_query(index_t *idx, char * directory, const char* arr[], int size);


#endif // __QUERIER_H


