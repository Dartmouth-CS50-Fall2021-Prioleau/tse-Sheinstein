/* 
 * pagedir.h - header file for 'pagedir' module
 *
 * 'pagedir' module comprising helper functions for 'crawler'
 * 
 * Salifyanji J Namwila Oct 11, 2020
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdbool.h>
#include "bag.h"
#include "hashtable.h"
#include "webpage.h"

/************************ pagefetcher *******************************/
/* Fetches a page from a URL.
 * Input: A URL whose page user wants to retrieve.
 * Returns true if the input URL's page is successfully returned.
 * Returns false if page cannot be fetched or any error occurs in the process. 
 */
bool pagefetcher(webpage_t *page);


/****************************** pagescanner ***************************/
/* Extracts URLs from a page and processes each one
 * Input: A page  from where URLs are to be extracted, 
 * a bag to add URLs to later visit, and a hashtable 
 * to add page after scanning it for more URLs.
 */
void pagescanner(webpage_t *page, hashtable_t *ht, bag_t *bag);


/***************************** pagesaver *******************************/
/* Outputs a page to the appropriate file.
 * `char` pointer to Directory of file to write to , int number of file and 
 * page to write in the file
 * Assumes file path name is no longer than 500 characters. 
 */
void pagesaver(char *directory,int document_ID, webpage_t *page);

#endif // __PAGEDIR_H
