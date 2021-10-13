/* 
 * pagedir.c - CS50 'pagedir' module
 *
 * see pagedir.h for more information.
 *
 * Salifyanji J Namwila Oct 11, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"
#include "webpage.h"

/**************** helper functions from `pagedir.h implemented here ****************/

/***************************8* pagefetcher ********************************/
bool pagefetcher(webpage_t *page)
{
    char *url = webpage_getURL(page); // get URL(also link to another page) from page
    if (!IsInternalURL(url)) { 
        fprintf (stderr, "URL is not internal i.e does not belong to http://cs50tse.cs.dartmouth.edu/ domain\n"); 
        return false;
    }
    else if (!webpage_fetch(page)) { // here is where we get 1-second delay after everyh fetch attempt
        fprintf(stderr, "could not fetch webpage from input URL\n");
        return false;
    }
    return true;
}

/************************ pagescanner *********************************/
void pagescanner(webpage_t *page, hashtable_t *ht, bag_t *bag)
{
    int pos = 0;
    char *result;
    //print rogress indicator here
    printf("%d     Scanning:     %s\n", webpage_getDepth(page),webpage_getURL(page)); // scanning URL progress indicator 

    // parse the webpage to extract all embedded URLs
    while ((result = webpage_getNextURL(page, &pos)) != NULL) {
        if (NormalizeURL(result)) {
            if (IsInternalURL(result)) { // double checks indirectly if  parsed URL is normalized url  AND also internal
                // try inserting URL into hashtable of seen URLs
                if (hashtable_insert(ht, result, " ")) {
                    // make a new webpage for URL, at depth+1
                    webpage_t *new = webpage_new(result, webpage_getDepth(page) + 1, NULL); //later freed after being scanned in crawler
                    if (new != NULL) {
                        // add the new webpage to the bag of webpages to be crawled
                        bag_insert(bag, new);
                        printf("%d     Found:     %s\n", webpage_getDepth(new), webpage_getURL(new)); // found new URL status indicator 
                    }
                }
                else {
                    //printf("Found seen URL: %s\n", result); // found seen URL status indicator  
                    free(result); // remember to free malloc'd memory
                }
            }
            else {
                //printf("Found external URL: %s\n", result); // found external URL status indicator 
                free(result); // remember to free malloc'd memory
            }
        }
        else free(result); // remember to free malloc'd memory
    }
}

/************************ pagesaver ***********************/
void pagesaver(char *directory,int document_ID, webpage_t *page)
{
    if (NormalizeURL(webpage_getURL(page)))
    {
       char filepath[300];
       sprintf(filepath, "%s/%d", directory, document_ID); // create path name
       FILE *fp = fopen(filepath, "w");
    
        if (fp != NULL) {
           fprintf(fp, "%s\n", webpage_getURL(page)); // write URL to file
           fprintf(fp, "%d\n", webpage_getDepth(page)); // write tdepth to file
           fprintf(fp, "%s\n",webpage_getHTML(page)); // write html to new file
           fclose(fp);
           printf ("%d     Saved:     %s\n", webpage_getDepth(page), webpage_getURL(page)); // saving progress indicator
        }
    else fprintf(stderr, "FILE POINTER ERROR: Failed to open file with NULL pointer\n");
    }else fprintf(stderr, " Saver could not normalize page\n");
}

