# Salifyanji J Namwila
## CS50 Fall 2021, Lab 5-TSE `indexer` Design

### User interface 

The indexer's only interface with the user is on the command-line; it must always receive two arguments.

```
./indexer pageDirectory indexFilename
```

For example:

``` bash
$ ./indexer ../tse-output/toscrape-depth-0 ../tse-index-output/toscrape-depth-0
```

### Indextest Usage

The indextest's only interface with the user is on the command-line; it must always receive two arguments.

```
./indextest oldIndexFilename newindexFilename
```

For example:

``` bash
$ ./indextest ../tse-index-output/toscrape-depth-0 ../tse-indextest-toscrape-depth-0
```

### Indexer Inputs

Input: The only inputs for indexer are command-line parameters; see the User Interface above.

### Indexer Outputs
Output: Upon reading the documents produced by the TSE crawler and building an index, indexer writes that index to a file. The newIndexFilename command line argument  is uased as the file name. Within a file, the following are  written:
 * one line per word, one word per line
 * each line provides the word and one or more (docID, count) pairs, in the format
    * word docID count [docID count]...

 ### Indextest Inputs
 Input: The only inputs for indextest are command-line parameters; User Interface section of this documentation details the formating of inputs on the command line .

### Indextest Outputs
Output (indextest): Upon loading an index file produced by the indexer into an index data structure, `indextest` saves that index to another file. The newIndexFilename command line argument as the file name. Files are written in the same format as files produced by indexer.
 
### Functional decomposition into modules

For indexer, the following modules or functions are anticipated:
   1. `main`, which parses arguments and initializes other modules
   2. `index_build`, which takes in each file and processes the words contained in the file
   3. `normalize_word`, which takes in a word changes all its letters to lowercase 
   4. `index_save`, which saves the inverted index to a file

And some helper modules that provide data structures:
   1. `hashtable` (as the index) 
   2. `set` (built into hashtable)
   3. `counterset` (of docID count pairs)

For indextest, the following modules or functions are anticpated:
   1. `main`, which parses arguments and initializes other modules
   2. `index_load`, which loads file created by indexer into an index data structure
   3. `index_save`, which saves the the loaded index to another file

Indextest leverages the same helper modules (that provide data structures) as indexer.

### Pseudo code for algorithmic flow

The indexer runs as follows:

1. Check and validate parameters
2. Create the index (initialize an empty hashtable)
3. Build the index
4. For each document in the crawler directory:
    5. Read the URL, depth, and HTML to create a new webpage struct
    6. For each word in the webpage:
        7. If it has three or more letters, normalize
        8. Check if that word has already been found
            9. If it is the first time coming across the word, add it to the hashtable with the corresponding docID and count = 1
            10. If the word already exists in the hashtable for that docID, increment its count
11. Save the index
12. For each word in the hashtable:
    13. Obtain each of its [docID count] pair and write to the them to `newIndexFilename`, one word per line, with the word at the beginning of the line followed by each of its [docID count] pairs

### Dataflow through modules

   1. main parses parameters and passes them to *indexbuild*.
   2. *indexbuild* takes each file and processes the words contained in the file. When *indexbuild* finds a word, it passes it to `normalize_word`, then stores the result in the hashtable and updates the corresponding counts. The entire hashtable, once populated, is then passed to *indexsave*.
   4. *indexsave* saves the inverted index to a file.

### Testing plan

Assemble the indexer and test it as a whole using testing.sh. In each case, examine the output indexFilename carefully to be sure that the file is in the following format:
 * one line per word, one word per line
 * each line provides the word and one or more (docID, count) pairs, in the format
    * word docID count [docID count]…
 * where word is a string of lower-case letters,
 * where docID is a positive non-zero integer,
 * where count is a positive non-zero integer,
 * where the word and integers are separated by spaces.

1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly. These cases are commented in `testing.sh`.
2. Compare the index produced by `indexer` with the index loaded by `indextest`.

### Assumptions

 1. pageDirectory has files named 1, 2, 3, ..., without gaps.
 2. The content of files in pageDirectory follow the format as defined in the specs.

### Optional Choices

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

### Limitations

1. #### Testing 
As with crawler, the user has to manually compare  the outputs either indexer's or indextest's out[utswith the course website's outputs to verify that they are correct. The outputs can be found here [ http://cs50tse.cs.dartmouth.edu/tse-output/]
