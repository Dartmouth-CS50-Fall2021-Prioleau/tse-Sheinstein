 Salifyanji J Namwila
## CS50 Fall 2021, Lab 6: TSE Querier Testing


### Testing plan

*Integration testing*. Assemble the querier and test it as a whole using `testing.sh`. In each case, examine the output to be sure docID score pairs were determined correctly.

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


### Limitations
1. #### Testing Outputs Verification
As with crawler and Querier, the user has to manually compare  the querier outputs with the indexer's output by counting the expected output manually. The indexer outputs can be found here [ http://cs50tse.cs.dartmouth.edu/tse-output/]



3. Manual verification of correct performance of program.

#### querier.c Unittesting
`qnittest` IS DEFINED IN `qunittest.h`. A glassbox of `queirer.c` unittesting is implemented .This way, index.c can run as a program if the unit test is defined. In `querier.c` two  main methods a re defined such that the unit main method runs only when `IFDEF UNITTEST` is defined. Otherwise, the default querier main method runs. To run unittest user should simply run `make qunittest` or `make unit` in the querier directory. To run valgrind on the unittest, uncomment the `valgrind `section for `qunittest`.  The program test guarantees absolute zero memory leaks.Furthermore, the program effectively tests some of the  statics helper methods the methods/ functions that querier.c has to offer. 

##### Running Querier Unittest - *qunittest*
The following are some importants remarks to runninng unnitest:

1. To run the unittest, simply call `make qunittest` or `make qunit`.  
3. To rid off all object files, simply run `make clean`.


