# Lab 6 rubric

## Elorm Coch

## Total: 96/100
## (10/10) Delivery
  * (3) Proper git tag `lab6submit`.   
  * (2) git commit history - logical groupings of changes in each commit, and meaningful commit messages.
  * (3) Proper file structure: tse directory and subdirectories `querier` and `common` exist, and `querier` must include appropriate `.gitignore`.  
  * (2) no executable files, object files, core dump files, editor backup files, or extraneous files existing in any directory.


## (12/12) Makefiles
  * (0) `tse/Makefile` should now build all subdirectories.
  * (4) `querier/Makefile` 
	  * (1) exists
	  * (1) has proper target to build `querier `
	  * (0) optionally has target to build `fuzzquery`
	  * (1) uses standard CFLAGS (or with `c11` changed to `gnu11`).
	  * (1) has an appropriate `clean` rule.
  * (4) directory `common/` exists and `common/Makefile`
	  * (1) exists 
	  * (1) has proper targets to build `common.a`
	  * (1) uses CS50 standard CFLAGS.
	  * (1) has an appropriate `clean` rule.
  * (4) Builds `querier` 

## (12/12) Documentation
In `tse/querier/`,
  * (2) `README.md`, which says how to build `querier`.
  * (4) `DESIGN.md`, which describes the abstract design of the querier with no language-specific details.
  * (4) `IMPLEMENTATION.md`: describes the implementation of `querier`.
  * (2) `testing.sh`

## (6/6) Testing
  * (5) `tse/querier/testing.sh`
  * (1) `tse/querier/testing.out`

## (20/20) Coding Style
  * [CS50 C program coding style](http://www.cs.dartmouth.edu/~cs50/Resources/CodingStyle.html)
  * Their querier-specific code should be isolated from the data structure code. 
  * Their querier code should not 'peek' inside opaque types.
  * They *need not* use my [*parse_args*]({{site.lectures}}/parse_args) approach, but *should* have a clean, well-organized code for parsing and validating command-line args that is not interwoven with the overall program flow & logic.
  * The code should be rigorous in checking for memory-allocation errors or file I/O methods. They are allowed to use `assertp()` or similar methods to exit when `malloc` or `fopen` or constructors (e.g., index_new()) fails or other sanity checks fail.
  * Well-designed code structure
    * avoid deep nested loops or conditional blocks.
    * consistent indentation and spacing.
    * sufficient comments. 

## (30/30) Functionality
  * Passed test cases for valid and invalid cases

## (6/10) Memory leaks, according to valgrind
  * (4) free of *memory errors*. 
  * (2) free of *still reachable* memory blocks. 
  * (-4) free of *lost memory blocks*.
    - leaks memory when no matches are found. 
    - Lines 320 - 323 in `querier.c` returns abruptly if `num_matches = 0` withouth free `result` counters.
    * Command: `./querier ../../data/tse-output/letters-depth-6 ../../data/tse-output/letters-index-6`
    * Result:
        ```
        Loading index...
        Query? years
        query:  years
        no matches
        -----------------------------------------------------------------------------------
        Query? Cleaning up...
        Done.
        ==13339== 
        ==13339== HEAP SUMMARY:
        ==13339==     in use at exit: 8 bytes in 1 blocks
        ==13339==   total heap usage: 187 allocs, 186 frees, 16,190 bytes allocated
        ==13339== 
        ==13339== 8 bytes in 1 blocks are definitely lost in loss record 1 of 1
        ==13339==    at 0x4C31B0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
        ==13339==    by 0x10CEDC: count_malloc (memory.c:53)
        ==13339==    by 0x10C4D6: counters_new (counters.c:39)
        ==13339==    by 0x109D73: counters_sum (querier.c:404)
        ==13339==    by 0x109A9F: run_query (querier.c:315)
        ==13339==    by 0x1099B1: process_query (querier.c:290)
        ==13339==    by 0x1095FE: main (querier.c:168)
        ==13339== 
        ==13339== LEAK SUMMARY:
        ==13339==    definitely lost: 8 bytes in 1 blocks
        ==13339==    indirectly lost: 0 bytes in 0 blocks
        ==13339==      possibly lost: 0 bytes in 0 blocks
        ==13339==    still reachable: 0 bytes in 0 blocks
        ==13339==         suppressed: 0 bytes in 0 blocks
        ==13339== 
        ==13339== For counts of detected and suppressed errors, rerun with: -v
        ==13339== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
        ```
