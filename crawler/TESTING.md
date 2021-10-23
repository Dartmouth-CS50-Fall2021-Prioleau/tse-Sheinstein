# Salifyanjin J Namwila
## CS50 Fall 2021, Lab 4: TSE Cralwer Testing

### Testing
Crawler is tested by running `testing.sh` (integration testing) from the command line after `crawler` has been assembled. `testing.sh` tests the crawler's response to invalid arguments, an incorrect number of arguments,
different depths, and different seed URLs. To confirm that crawlere has crawled the bag of webpages successfully, `testing.sh` checks if the correct number of files(document_IDs) have been created in the input `pageDirectory`. 

To ensure that no pages are missing or duplicated, `crawler` also prints the following "progress" indicators as it proceeds and the depth at which each of those progresses are reported:
  
  1. Saving URL: `URL`
  2. Exploring URL: `URL` 
  2. Scanning URL: `URL`
  3. Found new URL: `URL`
  4. Found seen URL: `URL`
  5. Found external URL: `URL`
  
### Testing plan


`Integration testing`. Assemble the crawler and test it as a whole. In each case, examine the output files carefully to be sure they have the contents of the correct page, with the correct URL, and the correct depth. Ensure that no pages are missing or duplicated. Print "progress" indicators from the crawler as it proceeds (e.g., print each URL explored, and each URL found in the pages it explores) so you can watch its progress as it runs.

    0. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

    1. Test the crawler with a seedURL that points to a non-existent server.

    2. Test the crawler with a seedURL that points to a non-internal server.

    3. Test the crawler with a seedURL that points to a valid server but non-existent page.

    4. Crawl a simple, closed set of cross-linked web pages. Ensure that some page(s) are mentioned multiple times within a page, and multiple times across the set of pages. Ensure there is a loop (a cycle in the graph of pages). In such a little site, you know exactly what set of pages should be crawled, at what depths, and you know where your program might trip up.

    5. Point the crawler at a page in that site, and explore at depths 0, 1, 2, 3, 4, 5. Verify that the files created match expectations.

    6. Repeat with a different seed page in that same site. If the site is indeed a graph, with cycles, there should be several interesting starting points.

    7. Point the crawler at our Wikipedia playground. Explore at depths 0, 1, 2. (It takes a long time to run at depth 2 or higher!) Verify that the files created match expectations.

    8. When you are confident that your crawler runs well, test it on a part of our playground or with a greater depth - but be ready to kill it if it seems to be running amok.

### Limitations
1. The user must manually scan the webpages to determine if they were explored correctly, which gets acutely tedious with more pages     explored. The user may also run the example cases listed [here] http://cs50tse.cs.dartmouth.edu/tse-output from the command line and compare the crawler output with those examples. The user should keep in mind that crawler may  process URLS in different order(items from bag are extracted randomly). Ergo, user's directory may not be identical to the one on the site listed

2. Specific to to the testing.sh, the directories created during the crawl have to be manually deleted. It is an ugly process. To make it less tedious, User can name the directories to contain word `depth` so that once the directories are created and Uers wants to rid them off at once, they can just type `rm -R *depth* `to remove all depth word containing directories from the crawler directories. For Users sake, they can make directory names to contain any word uniques only to the directories. The point is once the command `rm -R *xxxxx* ` is called, it deletes only those directories.
 Alternatively, and actually more conveniently, which I implemented in my testing.sh, user can just add `-rm -R *depth*` to the crawler `Makefile`. Here the user must be sure to only call  `make clean`  after running `make test` to avoid missing the target.

3. Manual verification of correct performance of program.

#### Index.c Unittesting
`Unittest` IS DEFINED IN `unittest.h`. A glassbox of `index.c` unittesting is implemented .This way, index.c can run as a program if the unit test is defined. To run unittest user should simply run `make unittest` or `make unit` in the common directory. IMPORTANT:User should however ensure that they run crawler and have the `../tse-output/letters-depth-1` directory prodeced by crawler. This directory is is what is primarily used for testing the functionality of each one of the module's components. Failure to have the aforementioned driectory will lead to acute failure of the unit test program. Otherwise, the program effectively tests all the methods/ functions that index.c has to offer. To run valgrind on unittest, simply call `make valgrind`. The program test guarantees absolute zero memory leaks.

##### Running Index Unittest
The following are some importants remarks to runninng unnitest:
1. Make sure crawler is run first to produce  `../tse-output/letters-depth-1` directory on which parts of the  unittesting  is based.
2. To run the unittest, simply call `make unittest` or `make unit`. This call produces two files within the common directory namely `unittest-saved-index` and `unittest-loaded-index`. 
3. To rid off all object files and the two unittest files, simply run `make clean`.


