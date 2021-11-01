#!/bin/bash
# testing.sh -- testing file for querier
# Usage: make test
# Input: none
# Output: Testing results, see testing.out for more specific results
#Verification Method: "Manual verification of files -- output matches indexer file count of corresponding query expectations"
#
# Author: Salifyanji J Namwila October 31 2021
# CS50 Fall 2021.
# Based on testing script by Temi Prioleau
# Feb 5, 2020
#
#QUERIER ARGUMENTS: ./querier pageDirectory indexFilename
#example: ./querier   ../tse-output/letters-depth-2  ../tse-index-output/letters-index-2



chmod +x testing.sh # change mode of .out file to executable


#  make parent directory outside cralwer to hold all new directories made in this test
mkdir ../tse-querier-output

make

# argumet validation file

mkdir ../tse-querier-output/notCrawlerProduced

echo "====================================================================================="

echo "Integration testing for querier module."
echo "IMPORTANT: ensure you have tse-output and tse-index-output loaded before running the script."
echo "For details on how to loaded tse-output and tse-index-output, please see the general README."
echo "Currently using -DTEST flag to see progress indicators (e.g. current file ID)."
echo "(Comment out for brevity, see Makefile in common to delete the flag)"
echo "Use valgrind to check for memory leaks"

echo "====================================================================================="

#************************************* various Querier commandline arguments testing block ************************************#
### The tests cases defined in this block should  all fail ###
echo "Section 0: Testing: Parameter Checks"

echo "Testing querier (error-handling)"


# 2 arguments
echo "Error handling: Insufficient arguments"
./querier ../tse-output/toscrape-depth-0
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Insufficient arguments provided"

fi

# 4 arguments
echo "Error handling: Too many arguments"
./querier ../tse-output/letters-depth-0 ../tse-index-output/wikipedia-index-0 ../tse-index-output/wikipedia-index-1
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Too many arguments provided"

fi



# invalid pagedirectory (not crawler-produced)
echo "Error handling: Testing indexer on invalid pagedirectory (not crawler-produced)"
./querier ../tse-querier-output/notCrawlerProduced ../tse-index-output/letters-index-0
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Indexer requires a crawler-produced directory"

fi

# invalid pagedirectory (non-existent)
echo "Error-handling: Testing crawler on invalid pagedirectory (non-existent)"
./querier ../tse-output-not-exist/ ../tse-index-output/letters-index-0
if [ $? -eq 0 ]; then
    echo >&2 "Error caught: Indexer ignores a non-existent pagedirectory"

fi

# make invalid directory (no read permission)
mkdir ../tse-querier-output/notReadable
chmod -r ../tse-querier-output/notReadable

echo "Error-handling: Testing indexer on invalid directory (no read permission)"
./querier ../tse-output/letters-depth-0 ../tse-querier-output/notReadable/letters-index-0
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Directory must be Readable"

fi

# good practice to change directory back to readable
chmod +r ../tse-querier-output/notReadable


#*****************************************Query Input cases testing block **********************************#
echo "====================================================================================="

# The tests cases defined in this block should  all pass ###
echo " Section 1 Testing: Indexing letters, toscrape and wikipedia crawler produced directories  "

echo "Testing indexer on letters at depth 0, 1, 2, 3, 4, 5, 6"



echo "=========================================================="
echo "Testing querier on edge cases for letters at depth 6" 

echo "Testing querier on empty query"
echo "" | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does nothing and continues on empty query"
fi

# Testing querier on whitespace
echo "Testing querier on whitespace"
echo "   " | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does nothing and continues on query of only whitespace"
fi

# Testing querier with a starting operator
echo "Testing querier with a starting operator"
echo "and computational" | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow AND to be first in the query"
fi

echo "or search" | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow OR to be first in the query"
fi

# Testing querier with an ending operator
echo "Testing querier with an ending operator"
echo "computational and" | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow AND to be last in the query"
fi

echo "search or" | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow OR to be last in the query"
fi



# ************************************* Testing querier with consecutive operators ******************************** "
echo "====================================================================================="
echo" ********** Testing querier with consecutive operators ************** "

#  Testing querier with consecutive operators
echo "Testing querier with consecutive operators"
echo "home or and eniac" | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow AND and OR to be consecutive"
fi

echo "page or or eniac" | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow AND and AND to be consecutive"
fi

echo "breadth and and first" | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow OR and OR to be consecutive"
fi

echo "algorithm  biology and or " | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow OR and AND to be consecutive"
fi

# ************************************* Testing querier with uppercase query inputs and space variations *************************************"
echo "====================================================================================="
echo" *****Testing querier with uppercase query inputs and space variations ********"

#  Testing querier with uppercase query inputs
echo " Testing querier with uppercase query inputs"
echo "DEPTH AND eniac" | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow AND and OR to be consecutive"
fi


#  Testing querier with uppercase query inputs and  a lot of spaces
echo " Testing querier with uppercase query inputs and  a lot of spaces"
echo "     DEPTH     OR        eniac         " | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow AND and OR to be consecutive"
fi



#  Testing querier with uppercase query inputs and  a lot of spaces and implicit and sequences
echo " Testing querier with uppercase query inputs and  a lot of spaces and implicit and sequences"
echo "     DEPTH     OR        eniac    ALGORITHM       " | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow AND and OR to be consecutive"
fi



#  Testing querier with uppercase query inputs and  a lot of spaces before and after words and implicit andsequences
echo " Testing querier with uppercase query inputs and  a lot of spaces before and after words and implicit andsequences"
echo "    DEPTH FIRST SEARCH  " | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow AND and OR to be consecutive"
fi


# Testing querier on words not in index file
echo "Testing querier on words not in  index file"
echo "   computer or  science  and dartmouth  " | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3 
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier returns 'No documents match.' for words it cannot find."
fi



# ************************************************* Testing non-letter characters ********************************************
echo "====================================================================================="
 echo" ******* Testing  querier on non-letter characters ***********"

# Testing querier on non-letter characters

# non- letter starting query
echo "Testing querier on non-letter characters: non- letter starting query"
echo " 4 enaic  or breadth " | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3 
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow non-letter characters."
fi

# non- letter middle of  query
echo "Testing querier on non-letter characters: non- letter middle of query"
echo " enaic 5 6  or biology  breadth " | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3 
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow non-letter characters."
fi

# non- letter end  of  query
echo "Testing querier on non-letter characters: non- letter end  of query"
echo " enaic or biology  or breadth " | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3 
if [ $? -eq 0 ]; then
    echo >&2 "Error-handling: Querier does not allow non-letter characters."
fi

#*****************************************Regular(valid) cases testing block **********************************
echo "====================================================================================="
echo " ================= Regular(valid) cases testing block ================"

echo "================== fuzzquery testing ================================="
# The tests cases defined in this block should  all pass
echo " Section 2 Testing: Indexing letters, toscrape and wikipedia crawler produced directories  "

echo "Testing indexer on letters at depth 0, 1, 2, 3, 4"

# fuzz query letters test 
#  fuzz query letters depth 0
echo "======================================================================="
echo "Querier tests on letters at depth 0"



# fuzz query letters depth 1
echo "Testing querier on letters at depth 0" 
./fuzzquery ../tse-index-output/letters-index-0 30 7  | ./querier ../tse-output/letters-depth-0 ../tse-index-output/letters-index-0 
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on letters at depth 0."
    exit 1
fi


# fuzz query letters depth 1
echo "Testing querier on letters at depth 1" 
./fuzzquery ../tse-index-output/letters-index-1 30 77  | ./querier ../tse-output/letters-depth-1 ../tse-index-output/letters-index-1 
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on letters at depth 1."
    exit 1
fi

# fuzz query letters depth 2
echo "Testing querier on letters at depth 2" 
./fuzzquery ../tse-index-output/letters-index-2 30 777  | ./querier ../tse-output/letters-depth-2 ../tse-index-output/letters-index-2
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on letters at depth 2."
    exit 1
fi


# fuzz query letters depth 3
echo "Testing querier on letters at depth 3" 
./fuzzquery ../tse-index-output/letters-index-3 30 7777  | ./querier ../tse-output/letters-depth-3 ../tse-index-output/letters-index-3 
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on letters at depth 3."
    exit 1
fi


#fuzz query  letters depth 4
echo "Testing querier on letters at depth 4" 
./fuzzquery ../tse-index-output/letters-index-4 30 9  | ./querier ../tse-output/letters-depth-4 ../tse-index-output/letters-index-4 
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on letters at depth 4."
    exit 1
fi

echo "Testing querier on letters at depth 0, 1, 2, 3, 4, Complete"

echo "======================================================================="

# fuzz query wikipedia depths 0, 1, 2
echo "Querier tests on wikipedia at depth 0, 1, 2"

# fuzz query wikipedia depth 0
echo "Testing querier on wikipedia at depth 0" 
./fuzzquery ../tse-index-output/wikipedia-index-0 30 99 | ./querier ../tse-output/wikipedia-depth-0 ../tse-index-output/wikipedia-index-0 
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on wikipedia at depth 0."
    exit 1
fi

# fuzz query wikipedia depth 1
echo "Testing querier on wikipedia at depth 1" 
./fuzzquery ../tse-index-output/wikipedia-index-1 30 999 | ./querier ../tse-output/wikipedia-depth-1 ../tse-index-output/wikipedia-index-1
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on wikipedia at depth 1."
    exit 1
fi

# fuzz query wikipedia depth 2
echo "Testing querier on wikipedia at depth 2" 
./fuzzquery ../tse-index-output/wikipedia-index-2 30 9999  | ./querier ../tse-output/wikipedia-depth-2 ../tse-index-output/wikipedia-index-2 
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on wikipedia at depth 2."
    exit 1
fi

echo "Testing querier on wikipedia at depth 0, 1, 2 Complete"

echo "======================================================================="

# fuzz query toscrape depth 0, 1, 2
echo "Querier tests on toscrape at depth 0, 1, 2"

# fuzz query toscrape depth 0
echo "Testing querier on toscrape at depth 0" 
./fuzzquery ../tse-index-output/toscrape-index-0 30 10 | ./querier ../tse-output/toscrape-depth-0 ../tse-index-output/toscrape-index-0 
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on toscrape at depth 0."
    exit 1
fi

# fuzz query toscrape depth 1
echo "Testing querier on toscrape at depth 1" 
./fuzzquery ../tse-index-output/toscrape-index-1 30 100 | ./querier ../tse-output/toscrape-depth-1 ../tse-index-output/toscrape-index-1 
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on toscrape at depth 1."
    exit 1
fi

# fuzz query toscrape depth 2
echo "Testing querier on toscrape at depth 2" 
./fuzzquery ../tse-index-output/toscrape-index-2 30 1000| ./querier ../tse-output/toscrape-depth-2 ../tse-index-output/toscrape-index-2 
if [ $? -ne 0 ]; then
    echo >&2 "Querier failed on toscrape at depth 2."
    exit 1
fi

echo "Testing querier on toscrape at depths 0, 1, 2 Complete."



echo "=========================== End of testing.sh ===================="

exit 0

