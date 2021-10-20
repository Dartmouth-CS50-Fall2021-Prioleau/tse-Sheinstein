
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

### Compilation

To build, run `make`.

To test, run `make test`

To clean up, run `make clean`.

NOTE: Infor amtion about role and implementation of each of the 2 parts so far can be found in the README.md file in their respective sub directories.
