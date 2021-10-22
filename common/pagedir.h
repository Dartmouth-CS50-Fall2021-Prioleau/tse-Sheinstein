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
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"

/************************ pagefetcher *******************************/
/* Fetches a page from a URL.
 * Input: User enters  page whose URL to retrieve.
 * our method gets the URL from the page.
 * If the URL is not internal, an std error is printed and method returns false
 * If the URL is internal, try to fetch the page. If unsucceful, we print the  error  and retun false
 * Otherwise method return True.
 * Returns true if the input URL's page is successfully returned.
 * Returns false if page cannot be fetched or any error occurs in the process. 
 */

bool
pagefetcher(webpage_t *page);


/****************************** pagescanner ***************************/
/* Extracts URLs from a page and processes each one
 * User provides  page  from where URLs are to be extracted, 
 * a bag to add URLs to later visit, and a hashtable 
 * to add page after scanning it for more URLs.
 * While the bag is not empty,
 * We create a pointer to the Next URL once extracted and later free that point after internal
 * processing.
 * If the extracted URL is internal, we try to add to the hashtable, if we succeed, we create a new pointer
 * to the webpage one depth higher. 
 * User is responsible for calling free on this pointer.
 * we finall add the page to the bag to later explore and print progress report to user
 * if the page is not internal or normalised, we  print apprepriate massage to console and free the pointer we creted 
 */

void
pagescanner(webpage_t *page, hashtable_t *ht, bag_t *bag);


/***************************** pagesaver *******************************/
/* Outputs a page to the appropriate file.
 *User provides `char` pointer to Directory of file to write to , idocument ID of the file we are writing to and 
 * page to write in the file
 * We locally check if the givern directory exists or is writeable
 * if it is, we write the page to the file,
 * else we tell caller there was an error saving the URL
 * Assumes file path name is no longer than 500 characters. 
 */

void
pagesaver(char *directory,int document_ID, webpage_t *page);

/************** isCrawlerDirectory() **************/
/*Input: User provides a directory name
 * We Verify whether given directory is indeed a Crawler-produced directory
 * We create the pathname, dir/.crawler,  and store it on the stack file and so do not need to free it.
 * program tries to open the file for reading.
 * If the file opens, then dir is a Crawler-produced directory and we return true to caller.
 * Else we print aa stderr stating that we cannot verify the given directory is crawler produced.
 * and return false.
 */ 
bool
isCrawlerDirectory(char* directory);

#endif // __PAGEDIR_H
