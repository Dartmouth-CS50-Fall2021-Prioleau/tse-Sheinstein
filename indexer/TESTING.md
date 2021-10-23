# Salifyanjin J Namwila
## CS50 Fall 2021, Lab 5: TSE Cralwer Testing

### Testing
Indexer  is tested by running `testing.sh` (integration testing from the command line after indexer has been assemlbed. `testing.sh` tests the indexer's response to inv;aid arguments, an incorrect number of arguments, valid page directories and non existent pagedirectories. To confirm that indexer has indexed the files successfully, `testing.sh` checks if the output index files' contents match with outputs on in the path `1/thayerfs/courses/21fall/cosc050.02/workspace/cs50tse/tse-output`, if user is on  dartmouth thayer school plank and ` http://cs50tse.cs.dartmouth.edu/tse-output/` the official website rep for all outputs.

To ensure that indexer is doing following the right steps in producing the index, `indexer` prints the following progress indicators :
1. Building index..
2. Saving index ..
3. Loading index ..
4. Deleting index ..

### Cases Tested and Outputs:
Some error cases:
* nonexistent directory
* wrong number of arguments
* dirName given is not a directory

`indexer` and indextest perform succesful indexing of pages contained in the crawler-produced directory `tse-output`

Ouput of `testing.sh` script from running `make test` in indexer (`bash -v testing.sh &> testing.out` underlying call in indexer Makefile) can be found in `testing.out` file :

./test_indexer.sh &> test_script.out

testing.out  also produced the files `tse-index-output` (and `tse-index-sorted` after calling `indexsort.awk`), which is the result of running indexer, and `tse-indextest-output`(and `tse-index-sorted` after calling `tse-sorted-indextest`) , which is the file produced by indextest after loading the index from `indexer.out`

### indextest
index tester `indextest`, loads an index file produced by the indexer and saves it to another file. 

Its outputs are  sorted in to a new file using a handy script provided by the lab and then compared to the sorted version of those of indexer.
The following section  reiterates the details of the usage of `indexsort.awk` script in conjuction with testing.sh to ensure robust code for indexer and indextest:

### Teshing.sh and insexsort.awk - `which bin?` potential overlook.
(...also reiterated in main README.md)

The Lab website provided optional `indexsort.awk` intended to be used  in conjuction with testing.sh to sort indexer  and indextest outputs to allow for level comparrison. In lieu of making my own sorting script, I made use of the indexsort.awk script file. It  has the following Use description:

```c
/usr/bin/awk -f
 Sort the document pairs on each line of a TSE index file,
 and pipe output to `sort` so we sort the lines alphabetically.

 usage example: ./indexsort.awk indexFilename > indexFilename.sorted
 if that does not work, you may wish to try
   gawk -f indexsort.awk indexFilename > indexFilename.sorted 
```

NOTE: User needs to make sure they enter the right bin their terminal is working on before executing the file. To check the bin user is on, simply type and enter `which bin` on the terminal and copy the output in the first line of the `indexsort.awk` script. For example if user is on `/usr/bin/` bin, they should enter  `/usr/bin/awk -f` in the first line of `indexsort.awk`. Similarly, if on `/usr/local/bin/`, enter  `/usr/local/bin/awk -f`.

Failure to enter correct bin will result in `indexsort.awk`failure to work altogether.

