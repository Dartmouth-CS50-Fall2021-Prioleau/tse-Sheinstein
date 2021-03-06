/* 
 * crawler.c - CS50 'crawler' module
 *
 * see README.md for more information.
 *
 * Salifyanji J Namwila, Oct 11, 2021
 *
 */

/***************************************** INCLUDE FILES/LIBRARIES ****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/bag.h"
#include "../libcs50/memory.h"
#include "../libcs50/hashtable.h"
#include "../common/pagedir.h"
#include "../libcs50/webpage.h"


/****************************************FUNCTION PROTOTYPES****************************************/
void crawler(hashtable_t *ht, bag_t *bag, char* dir, int depth);
void item_delete();

/***********************************************MAIN*************************************************
* 
 * User inputs arguments which are checked for validity.
 * Method does not assume the User inputs the arguments correctly all the time
 * so each arg[n] is checked for validation before parsing them to other methods
*/
 
int main(int argc, char *argv[]) 
{   
    // make return status =1 for unsuccessfully returns from main for easy testing in `testing.sh`
    const int NUMSLOTS = 1000; // size of hashtable 
    int max_depth;
    
    // First validate commandline arguments
    if (argc != 4) {
        fprintf (stderr, "Incorrect number of arguments\n"); // check number of commandline arguments
        return 1;
    }
    if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) { // check if possibly NULL arguments != NULL
        fprintf (stderr, "NULL ARGS ERROR: program, or seedURL, or page directory are NULL pointers\n");
        return 1;
    }
    if (sscanf(argv[3], "%d", &max_depth) != 1) { // check if forth arg on commandline is  a valid `int`
        fprintf (stderr, "MaxDepth must be an integer\n");
        return 1;
    }
    if (max_depth < 0) { // check if maxDepth is a pos int
        fprintf (stderr, "MaxDepth must be a positive integer\n");
        return 1;
    }

    if (max_depth > 15) max_depth = 15; // fix upper limit for acceptable maxdepth 
   

   // Next, Since I now have valid inputs. I check if arguments are 
   // something my program can can work with. `Mallocable`? NormalizedURL? Internal?
    //char *seed_URL = (char *)malloc((strlen(argv[1]) + 1)*sizeof(char)); // allocate memory for seed URL
    //if (seed_URL == NULL) {
        //fprintf (stderr, "Could not allocate memory for seed_URL %s \n", argv[1]);
        //free(seed_URL);
        //return 1;
    //}

    char* seedURL = argv[1];
    if (!NormalizeURL(seedURL)) {
        fprintf(stderr, "Error normalizing seedURL %s\n", seedURL);
        return 1;
    }
    
     if (!IsInternalURL(seedURL)) { 
        fprintf (stderr, "URL is not internal\n");
        return 1;
        }

    char* seedURLcopy = assertp(malloc(strlen(seedURL)+1), "url copy");    
    strcpy(seedURLcopy, seedURL);
    

     // Next, determine whether the pathname provided as pageDirectory is indeed a directory                                                                    
     char *page_dir = argv[2]; // get path fron coomandline here                                                                                               
     char filename[strlen(page_dir)+ 10];// allocate an array of size 500 for filetest on stack                                                                                 
     //char* file = filename;                                                                                                                                  
     sprintf (filename, "%s/.crawler", page_dir); // create path                                                                                               
     FILE *fp = fopen(filename, "w"); // open created test file                                                                                                
     if (fp == NULL) {
         fprintf (stderr, "Directory does not exist or is not writable\n");
         free(seedURLcopy);
         return 1;
     }
    fclose(fp); // close test file if directory passed the validation                                                                                          

    //Next try `newing` all needed structures ready for parsing to  crawler: new webpage, new bag, new hashtable                                               
    webpage_t *seedpage = webpage_new(seedURLcopy, 0, NULL); // make a webpage for the seedURL, marked with depth = 0                                          
    if (seedpage == NULL) {
        webpage_delete(seedpage);
    
        fprintf (stderr, "Could not allocate memory for webpage_t\n");
        return 1;
    }

    bag_t *tocrawl = bag_new(); // create bag to hold webpages to be crawled                                                                                   
    if (tocrawl == NULL) {
        webpage_delete(seedpage);
        fprintf (stderr, "Bag_new failed\n");
        return 1;
    }
    bag_insert(tocrawl, seedpage); // add page for seedURL to the bag of webpages to crawl                                                                     

    hashtable_t *seen = hashtable_new(NUMSLOTS); // create hashtable to hold seen URLs                                                                         
    if (seen == NULL) {
        bag_delete(tocrawl, *webpage_delete);
        fprintf (stderr, "Hashtable_new failed\n");
        return 1;
    }
    hashtable_insert(seen, webpage_getURL(seedpage), ""); // add seedURL to the hashtable of seen URLs                                                         

    crawler(seen, tocrawl, page_dir, max_depth); // crawl webpages                                                                                             

    bag_delete(tocrawl, webpage_delete); // clean up                                                                                                          
    hashtable_delete(seen, NULL);

    return 0;
}

 /******************************************FUNCTIONS IMPLEMENTATED ********************************************

            *****************************************CRAWLER***************************************
 * Crawl webpages.
 * Errors and additional progress
 * indicators  are reported from `pagedir` to user
 */
void crawler(hashtable_t *ht, bag_t *bag, char* dir, int depth) {
    webpage_t *page; // get pointer first page
    int document_ID = 1; // keep track of file number (1 based index), increment later as you save other webpages

    while ((page = bag_extract(bag)) != NULL) { // while there are still webpages to crawl 
        if (pagefetcher(page)) { // retrieve a webpage for page URL. Pauses for 1-second after every fetch attempt
            // save the webpage to the pageDirectory with a unique document ID         
            pagesaver(dir,document_ID, page);
            printf ("%d  Exploring:     %s\n", webpage_getDepth(page), webpage_getURL(page)); // explore progress indicator
            document_ID++;
        }
        // if the webpage depth is < maxDepth, scan the webpage to for more URLs
        if (webpage_getDepth(page) < depth) {
            pagescanner(page, ht, bag); 
        }
        webpage_delete(page); // remember to delete malloc'd memory( after first page, all were allocated in `pagescanner`)
    }
}

/**************** item_delete ****************/
/* Delete each item in hashtable */
void item_delete(void *item)
{
    // nothing to delete since we did no malloc memory for the constant string ' ' items I chose to use.
}
