# Salifyanji J Namwila
## CS50 Spring 2020, Lab 5-TSE `indexer`

### indexer

The TSE **indexer** is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, *indextest*, loads an index file produced by the indexer and saves it to another file.

The *indexer* also prints "progress" indicators as it runs, indicating whether it is building or saving (or, in the case of *indextest*, loading) the index. `indexer` leverages modules from `libcs50-given.a`.

The indexer **shall**:

1. execute from a command line with usage syntax
  * `./indexer pageDirectory indexFilename`
  * where `pageDirectory` is the pathname of a directory produced by the Crawler, and
  * where `indexFilename` is the pathname of a file into which the index should be written; the indexer creates the file (if needed) and overwrites the file (if it already exists).
  * Note: the indexer should be able to handle cases in which the index file is not a valid pathname and/or the crawler directory is not a valid.
2. read documents and the neccessary content from the `pageDirectory`, each of which has a unique document ID, wherein
   * the document `id` starts at 1 and increments by 1 for each new page,
   * and the filename is of form `pageDirectory/id`,
   * and the first line of the file is the URL,
   * and the second line of the file is the depth,
   * and the rest of the file is the page content (the HTML, unchanged).
3. build an inverted-index data structure mapping from *words* to *(documentID, count) pairs*, wherein each *count* represents the number of occurrences of the given word in the given document.Ignore words with fewer than three characters, and "normalize" the word before indexing. Here, "normalize" means to convert all letters to lower-case.
4. create a file `indexFilename` and write the index to that file, in the format described below.

See [IMPLEMENTATION](IMPLEMENTATION.md) for details regarding `indextest`.

### Indexer Usage

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

### Compilation (of index and indextest)

To compile `indexer` and `indextest`, simply `make` or `make all`.

To compile only `indexer`, simply `make indexer`.

To compile only `indextest`, simply `make indextest`.

To test, simply `make test`, which redirects outputs of `testing.sh` into `testing.out`. Underlying instruction in `make test` in the Makefile  is `make test &> testing.out`.

To clean up directory, simply `make clean`.

Details of test run in [TESTING](TESTING.md).

Details of design and implementation choices in [DESIGN](DESIGN.md) and [IMPLEMENTATION](IMPLEMENTATION.md)respectively.

### Memory Leaks
The program is free of memory leaks with all the test cases provided in `testing.sh`

### Memory Allocation Choices
Where needed I allocated memory for objects on the stack to make debugging of memory leaks easy. However, whereever I used stack memeory, heap memory could be used instead, whuch is fundamentally much safer. If this code is to be repodused, user should consider using the heap to allocate memory and later call free on objects as need calls for it.