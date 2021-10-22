# Salifyanji J Namwila
## CS50 Fall 2021 Lab 4-TSE `crawler`

### crawler

The TSE crawler is a standalone program that crawls the web and retrieves webpages starting from a "seed" URL.
It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limits its exploration to a given "depth". The crawler does not explore pages in any particular order. This crawler limits its crawl to a specific web server inside Dartmouth CS.

Each explored webpage is saved to a file, one file per page, using a unique document ID as the file name (document IDs 1, 2, 3, 4, and so forth). Within a file, we write:
 * the page URL on the first line,
 * the depth of the page (where the seed is depth 0) on the second line,
 * the page contents, beginning on the third line.

The crawler also prints "progress" indicators as it runs, indicating whether it is exploring a new URL, has come 
across a URL it has already seen, or has found an external URL (which it will ignore). `crawler` leverages modules from `libcs50-given.a`.

The crawler **shall**:

 1. execute from a command line with usage syntax
   * `./crawler seedURL pageDirectory maxDepth`
   * where `seedURL` is used as the initial URL (must be an internal URL),
   * where `pageDirectory` is the pathname for an existing directory in which to write downloaded webpages, and
   * where `maxDepth` is a non-negative integer representing the maximum crawl depth (a maximal allowable value for `maxDepth` can be set in the code)
 2. crawl all pages reachable from `seedURL`, following  links to a maximum depth of `maxDepth`; where `maxDepth=0` means that crawler only explores the page at `seedURL`, `maxDepth=1` means that crawler only explores the page at `seedURL` and those pages to which `seedURL` links, and so forth inductively.
 3. pause at least one second between page fetches.
   * upon any error in fetching a page, log the error and continue to the next URL. 
   * ignore URLs that are not "internal" (meaning, outside the designated CS50 server).
 4. write each explored page to the `pageDirectory` with a unique document ID, wherein
   * the document `ID` starts at 1 and increments by 1 for each new page,
   * and the filename is of form `pageDirectory/id`,
   * and the first line of the file is the URL,
   * and the second line of the file is the depth,
   * and the rest of the file is the page content (the HTML, unchanged).
   * upon any error in writing the page to `pageDirectory`, log the error and continue.  

### Usage

The crawler's only interface with the user is on the command-line; it must always have three arguments.

```
./crawler seedURL pageDirectory maxDepth
```

For example:

``` bash
$ ./crawler http://cs50tse.cs.dartmouth.edu/letters/ ./tse-output/ 2
```
The pageDirectory can be entered as `./tse-output/` or `./tse-output`

### Compilation

To compile, simply `make`.

To test, simply `make test` or `make test &> testing.out`.

To clean up directory, simply `make clean`.

Details of test run in [TESTING](TESTING.md).

Details of implementation choices in [IMPLEMENTATION](IMPLEMENTATION.md).

### Memory Leaks
Unlike other Labs,which gracefully from memory-allocation failures. In the TSE application programs, the instructors of Lab 4 allowed for a  bit more lazy: on NULL return from malloc/calloc prints an error and exit with non-zero exit status. As such, it is ok if valgrind reports your program as having leaks. 
Of that whane valgrind is run on my test cases, it does report some blocks of memory lost.
