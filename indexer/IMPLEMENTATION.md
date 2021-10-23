# Salifyanji J Namwila
## CS50 Fall 2021, Lab 5-TSE `indexer` Implemetation

### Functions/modules

#### indexer

`indexer` is structured as follows:

  1. *main* parses parameters and initializes an empty index.

`indexer` uses following helpfer functions from the `common` library:
  1. *indexbuild*, which builds the index from `crawler` produced documents.
  2. *indexsave*, which saves the index to a file.

#### indextest

`indextest` is structured as follows:

1. *main* parses parameters and initializes an empty index.

`indextest` uses following helpfer functions from the `common` library:
  1. *indexload*, which populates an index data structure from an indexfile.
  2. *indexsave*, which saves the index to a file.

### Major data structures

 1. *hashtable* of words (as the index)
 2. *set* of countersets (indirectly used by hashtable)
 3. *counterset* of [docID count] pairs

### Pseudo code for algorithmic flow
#### indexer 

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
    13. Obtain each of its [docID count] pair and write to the them to `indexFilename`, one word per line, with the word at the beginning of the line followed by each of its [docID count] pairs

#### indextest

1. Check and validate parameters
2. Create the index (initialize an empty hashtable)
3. For each line in the first file:
    4. Use freadwordp to obtain the word and fscanf to loop through the pairs of “%d %d”
    5. For each word create a new counterset and set the key and value for each [docID count] pair
    6. Insert each key (word) item (counterset) pair into index’s hashtable
7. Write the contents of the inverted data structure to `newIndexFilename`

### Limitations

1. `Hashtable`:When building the inverted index, it’s impossible to know in advance how many words will be found in all the pages encountered by the crawler. A reasonable size of the hashtable is picked i.e. 500 in the range of 200..900 slots as per advice in the lab instructions.

