# Salifyanji J Namwila
## CS50 Fall 2021, Lab 5-TSE `querier` Design

### User interface 

The querier must always receive two arguments.

```
./querier pageDirectory indexFilename
```

For example:

``` bash
$ ./querier ../crawler/data ../indexer/indexfile
```

The querier will then request queries from the user via stdin until EOF.

### Inputs and outputs
Input: the inputs for querier are command-line parameters and queries received from stdin; see the User Interface above. An acceptable query consists of alphabet characters only. 'and' 'or' cannot appear at the beginning or end of the query, and cannot be adjacent within the query. 

Output: if the querier receives a valid query, that is a query without synax errors, it prints:
 * `No documents match` if the word is not contained in the indexFile
 * Otherwise... 
    * `Query:` and the query in lowercase format
    * `Matches x document(s):`
    * `score   x doc   x: URL`
    * `-----------------------------------------------`
        * Where... 
            * `score` is the number of times words in the query appear in a document
            * `doc` is the document that contains those words
            * `URL` is the URL from which that document was created
 
### Functional decomposition into modules
For querier we anticipate the following modules or functions:
   1. 




And some helper modules:
   1. hashtable (as the index) 
   2. set (built into hashtable)
   3. countersets (of docID docID, docID score, and docID count pairs)
   4. 

### Pseudo code for logic/algorithmic flow
The querier will run as follows:
1. 


### Dataflow through modules
1. 
### Testing plan

Assemble the querier and test it as a whole using testing.sh. In each case, examine the output to be sure docID score pairs were determined correctly.

1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly. These cases are commented in `testing.sh`.
2. Fuzz test the program with 50 random queries (accomplished by `testing.sh`).

Note: the querier assumes that:
 * pageDirectory has files named 1, 2, 3, ..., without gaps.
 * The content of files in pageDirectory follow the format as defined in the specs.
 * The content of the file named by `indexFilename` follows the [Index file format]({{site.labs}}/Lab5/REQUIREMENTS.html#index-file-format).
* Queries consist of words no longer than 50 characters.
* There are no more than 500 documents in the crawler directory.
