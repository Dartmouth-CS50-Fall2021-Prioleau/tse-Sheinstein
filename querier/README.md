# Salifyanji J Namwila
## ## CS50 Fall 2021 Lab 6-TSE `querier`

### querier

## Functionality

This *querier* prints the set of documents that contain all the words in the query in decreasing order by score, where the score is the number of occurrences of a word in a document. Operators (‘and’, ‘or’) are treated as regular words, but *querier* will print an error if 'and, 'or' appear at the beginning or end of a query, or are adjacent. 

## Overview

The TSE *querier* is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Crawler, and answers search queries submitted via stdin. Does not protect against duplicate queries.

The querier **shall**:

1. execute from a command line with usage syntax
  * `./querier pageDirectory indexFilename`
  * where `pageDirectory` is the pathname of a directory produced by the Crawler, and
  * where `indexFilename` is the pathname of a file produced by the Indexer.
2. load the index from `indexFilename` into an internal data structure.
3. read search queries from stdin, one per line, until EOF.
4. clean and parse each query according to the *syntax* described below.
5. if the query syntax is somehow invalid, print an error message, do not perform the query, and prompt for the next query.
5. print the 'clean' query for user to see.
5. use the index to identify the set of documents that *satisfy* the query, as described below.
6. if the query is empty (no words), print nothing.
6. if no documents satisfy the query, print `No documents match.`
7. otherwise, rank the resulting set of documents according to its *score*, as described below, and print the set of documents in decreasing rank order; for each, list the score, document ID and URL.
8. Exit with zero status when EOF is reached on stdin.

### Usage

The querier must always receive two arguments.

```
./querier pageDirectory indexFilename
```

For example:

``` bash
$ ./querier ../crawler/data ../indexer/indexfile
```

The querier will then request queries from the user via stdin until EOF.

### Assumptions

* `pageDirectory` has files named 1, 2, 3, ..., without gaps.
* The content of files in `pageDirectory` follow the format as defined in the specs for *crawler*.
* The content of the file named by `indexFilename` follows the *Index file format* in requirements.
* Queries consist of words no longer than 50 characters.
* There are no more than 500 documents in the crawler directory.

### Compilation

To compile `querier`, simply `make`.

To test, simply `make test` or `make test &> testing.out` after completing set-up detailed in [TESTING](TESTING.md).

To clean up directory, simply `make clean`.

  