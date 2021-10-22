
# Salifyanji J Namwila
## CS50 Fall 2021, Tiny Search Engine

GitHub username: Sheinstein

Tiny Search Engine (TSE) is a Dartmouth CS50 course project. The project is divided into three components, which  all towards  building a Tiny Search Engine.
The following are the components:
1. Crawler
2. Indexer
3. Querier

This lab focuses on implementing Indexer. Crawler was implemented in lab 4, while querier will be implemented in lab 6, ultimately making the TSE.

The following are functions of each of the components:
1. Crawler: Crawls through a website and retrieves webpages up to a given `depth`,extracts the embedded URLs, and writes them to a file in a given directory.
2. Indexer: Builds an `inverted index` from the `crawler produced directory`, mapping from words to document ID and count
3. Querier: Returns webpages based on the user's queries (supports 'and' and 'or' operators, which all look for unions or intersetions of words in given webpages )

### Important Optional Choices 

1. `libcs50.a` - Use of my self made modules from Lab 3 as was allowed in the instructions. In lieu of the starter kit provided `libcs50-given.a` archive, I use my own data structures from Lab 3 to create an archive `libcs50.a`. 
2. `indexsort.awk` - Use of optional use  provided .awk script in conjuction with testing.sh to sort indexer  and indextest outputs to allow for level comparrison. In lieu of making my own sorting script, I made use of the indexsort.awk script file. It  has the following Use description:

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

### Compilation

To build, run `make`.

To test, run `make test`

To clean up, run `make clean`.

NOTE: Infor amtion about role and implementation of each of the 2 parts so far can be found in the README.md file in their respective sub directories.
