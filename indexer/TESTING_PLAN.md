# Salifyanjin J Namwila
## CS50 Fall 2021, Lab 5: TSE Cralwer Testing
### Testing
Indexer  is tested by running `testing.sh` (integration testing from the command line after indexer has been assemlbed. `testing.sh` tests the indexer's response to inv;aid arguments, an incorrect number of arguments, valid page directories and non existent pagedirectories. To confirm that indexer has indexed the files successfully, `testing.sh` checks if the output index files' contents match with outputs on in the path `1/thayerfs/courses/21fall/cosc050.02/workspace/cs50tse/tse-output`, if user is on  dartmouth thayer school plank and ` http://cs50tse.cs.dartmouth.edu/tse-output/` the official website rep for all outputs.

To ensure that indexer is doing following the right steps in producing the index, `indexer` prints the following progress indicators :
1. Building index..
2. Saving index ..
3. Loading index ..
4. Deleting index ..

0. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

1. Test the indexer with a page directory that points to a non-existent page directory.

2. Test the indexer with a page directory that is not a directory.

3. Index a simple, closed set of cross-linked web pages i.e letters depth-4. Ensure that some page(s) are mentioned multiple times within a page, and multiple times across the set of pages. Ensure there is a loop (a cycle in the graph of pages). In such a little site, you know exactly what set of pages should be crawled, at what depths, and you know where your program might trip up.

4. index files produced at a page by crawler in that site, and index depths 0, 1, 2, 3, 4, 5. Verify that the files created match expectations.

6. Index all Repeat with a different files produced by crawler with different seed page in that same site. If the site is indeed a graph, with cycles, there should be several interesting starting points.

7. Index  Wikipedia playground files. Explore at depths 0, 1, 2. (It takes a long time to run at depth 2 or higher!) Verify that the files created match expectations.

### Limitations
1. The user must manually scan the files to determine if they were indexed correctly, which gets acutely tedious with more pages are indexed . The user may also run the example cases listed [here] http://cs50tse.cs.dartmouth.edu/tse-output from the command line and compare the cindexere and indextest  output with those examples. The user should keep in mind that indexer  may  index files  in different order. Ergo, user's directory  of indexer aand/.or index test may not be identical to the one on the site listed.

2. Specific to to the testing.sh, the directories created during the indexing have to be manually deleted by calling `make clean` in the directory above. 

3. Manual verification of correct performance of program.

