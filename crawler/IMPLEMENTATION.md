# Salifyanji J Namwila
## CS50 Fall 2021, Lab 4 Crawler Implementation
Drew instructions from CS50 TSE Crawler [Design Spec](https://www.cs.dartmouth.edu/~cs50/Labs/Lab4/DESIGN.html)

### Functions/modules

`crawler.c` is structured as follows:

  1. *main* parses arguments from commandline and initializes other modules i.e. new webpage for starter seedURL, new bag, and hashtable. The parsed argumnets and initialized modules are then passed to `crawler`.
  2. *crawler* uses a given bag to track pages to explore, and a given hashtable to track pages seen; when it explores a page, it passes that page URL to the pagefetcher, then the result to pagesaver, then to the pagescanner.

Crawler uses following helpfer functions from described in `pagedir.h` and implemented in `page.dir.c` both of which are found in the `common` library:
  1. *pagefetcher*, which fetches a page from a URL.
  2. *pagescanner*, which extracts URLs from a page and processes each one
  3. *pagesaver*, which outputs a page to the the appropriate file

### Major helper modules that provide data structures:

 1. *bag* of pages `crawler` is yet to explore
 2. *set* of URLs(indirectly used by hashtable)
 3. *hashtable* of URLs that have been seen so far (because the hashtable does not allow NULL items, I chose to have it store URLs as keys and a constant string, " ", as items as per suggest from the writer instructons)

### Pseudo code for logic/algorithmic flow

The crawler runs as follows:

1. execute from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules
3. make a *webpage* for the `seedURL`, marked with depth=0
4. add that page to the *bag* of webpages to crawl
5. add that URL to the *hashtable* of URLs seen
6. while there are more webpages to crawl,
	1. extract a webpage (URL,depth) item from the *bag* of webpages to be crawled,
	2. pause for at least one second,
	3. use *pagefetcher* to retrieve a webpage for that URL,
	4. use *pagesaver* to write the webpage to the `pageDirectory` with a unique document ID, as described in the Requirements.
	5. if the webpage depth is < `maxDepth`, explore the webpage to find links:
		1. use *pagescanner* to parse the webpage to extract all its embedded URLs;
		2. for each extracted URL,
			1. 'normalize' the URL (see below)
			2. if that URL is not 'internal' (see below), ignore it;
			3. try to insert that URL into the *hashtable* of URLs seen
				1. if it was already in the table, do nothing;
				2. if it was added to the table,
					1. make a new *webpage* for that URL, at depth+1
					2. add the new webpage to the *bag* of webpages to be crawled

**normalize the URL** means to convert it into a clean, canonical form.

**internal** means the URL stays within the CS50 playground http://cs50tse.cs.dartmouth.edu/tse/.

### Dataflow through modules

    1. *main* parses parameters and passes them to the crawler.
    2. *crawler* uses a bag to track pages to explore, and hashtable to track pages seen; when it explores a page it gives the page URL to the pagefetcher, then the result to pagesaver, then to the pagescanner.
    3. *pagefetcher* fetches the contents (HTML) for a page from a URL and returns.
    4. *pagesaver* outputs a page to the appropriate file.
    5. *pagescanner* extracts URLs from a page and returns one at a time.


### Limits and Assumptions

My implementation choice has the following limitations and assumptions:

1. Upper limit of allowable depth was set at 15. If the user enters a value over 15, max_depth is set to 15.
2. The hashtable is newed with a size = 1000. Essentially, my implementation assumes , or at least hopes we will not have more than a thousand seen webpages per seedURL. I believe it is a reasonable choice.
3. In order to check if the input directory name from commandline is exists or is writeable, I create a character pointer of cap size 500. This is with the assumption that the user does not write a directory name longer than 500. This too is reasonable deliberate choice.


