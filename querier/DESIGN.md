# Salifyanji J Namwila
## CS50 Fall 2021, Lab 6-TSE `querier` Design

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
For querier we anticipate the following  module functions:
1. *main*, which parses arguments and initializes other modules 
2. *get_tokens*, which extracts words from a line and normalizes it into its lower-case counterpart
3. *check_query_input*, which validates words in a query for placement of "and" and "or" conjunctions
4. *is_satify_query* which validates words in a query for placement of for absence of non-alpha characters
5. *process_query*, which processes the user's query from stdin
6. *run_query*, which handles the AND and OR sequences from the query and prints the output to stdout



And some helper modules:
   1. `hashtable` (as the index) 
   2. `set` (built into hashtable)
   3. `countersets` (of docID docID, docID score, and docID count pairs)

### Pseudo code for logic/algorithmic flow
The querier will run as follows:

```c
1. Execute from a command line with usage syntax `./querier pageDirectory indexFilename`
2. Validate its command-line arguments:
    1. Ensure `pageDirectory` is the pathname for an existing Crawler-generated directory by checking for the existence of .crawler
    2. Ensure `indexFilename` is the pathname of a readable, existing file
3. Load the index from `indexFilename` into an internal data structure.
4. Read search queries from stdin, one per line, until EOF.
	1. clean each query by changing the query to its lower case variation
    2. if the query syntax is somehow invalid, print an error message, do not perform the query, and prompt for the next query.
	3. print the 'clean' query for user to see.
	4. use the index to identify the set of documents that satisfy the query
        1. Check the next word in the query (if any)
            1. If the word is an OR, combine the current query into the final counters set
            2. Otherwise, add the current query to the temporary counters set of AND sequences
        2. If there are no more words and the temporary counters set is not empty, merge the temporary counters set into the final set by summing over the values
	5. if the query is empty (no words), print nothing.
	6. if no documents satisfy the query, print `No documents match.`
5. Rank the resulting set of documents according to its score, and print the set of documents in decreasing rank order; for each, list the score, document ID and URL.
    1. Obtain the URL by reading the first line of the relevant document file from the `pageDirectory`.
6. Exit with zero status when EOF is reached on stdin.

```

### Exit condition for Valid Query

The querier completes and exits when there are no more words from stdin to traverse through, or when the user has stopped the program (e.g. using ctrl-D).



### Dataflow through modules
1. *main* parses parameters, loads the index, and passes them to *process_query*
2. *process_query* reads through each line, extracts alpha words, checks if  they satisfy AND OR sequences, and prints normalized words to stdout (using *get_toekens*, *is_satisfy_query*,  *normalize_word*), before running the query


### Testing plan

*Integration testing*. Assemble the querier and test it as a whole using testing.sh. In each case, examine the output to be sure docID score pairs were determined correctly.

1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly. These cases are commented in `testing.sh`.
2. Fuzz test the program with 50 random queries (accomplished by `testing.sh`).

Break down:
0. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

1. Test the querier with a nonexistant `pageDirectory`.

2. Test the querier with a non-crawler generated `pageDirectory`.

3. Test the querier with a non-readable `indexFilename`.

4. Test the querier on an empty query.

5. Test the querier on a query with only whitespaces.

6. Test the querier with a starting conjunction.

7. Test the querier with an ending conjunction.

8. Test the querier with consecutive conjunctions.

9. Test the querier with words not present in the index file.

10. Test the querier with non-letter characters.

11. Point the querier to letters. Test the querier on one word, basic phrases combined by conjunctions, conjunction precedence (AND over OR), and using fuzzquery. Ensure that all results meet expectations, print the correct docIDs, counts, and URLs. 

12. Point the querier to toscrape and wikipedia at different depths and test with different combinations of phrases in addition to fuzzquery.

13. More tests as needed! (In this case, many fuzztests.)


### Assumptions:
 * pageDirectory has files named 1, 2, 3, ..., without gaps.
 * The content of files in pageDirectory follow the format as defined in the specs.
 * The content of the file named by `indexFilename` follows the [Index file format]({{site.labs}}/Lab5/REQUIREMENTS.html#index-file-format).
* Queries consist of words no longer than 50 characters.

### Limitations
1. #### Testing Outputs Verification
As with  the `crawler` and `indexer`, the user has to manually compare  the querier outputs with the indexer's output by counting the expected output manually. The indexer outputs can be found here [ http://cs50tse.cs.dartmouth.edu/tse-output/]

