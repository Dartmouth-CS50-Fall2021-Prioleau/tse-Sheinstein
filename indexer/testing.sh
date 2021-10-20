#!/bin/bash
# testing.sh -- testing file for indexer
# Usage: make test
# Input: none
# Output: Testing results, see testing.out for more specific results
#Verification Method: "Manual verification of files -- files match expectations"
#
# Author: Salifyanji J Namwila October 19 2021
# CS50 Fall 2021.
# Based on testing script by Temi Prioleau
# Feb 5, 2020
#
#INDEXER ARGUMENTS: ./indexer pageDirectory indexFilename
#example: ./indexer   ../tse-output/toscrape-depth-2  ../tse-index-output/toscrape-depth-2

#INDEXTEST ARGUMENTS: ./indextest oldIndexFilename newIndexFilename 
#example:  ./indextest ../tse-index-output/toscrape-depth-2  ../tse-index-sorted-output/toscrape-depth-2 

chmod +x testing.sh # change mode of .out file to executable

#  make parent directory outside cralwer to hold all new directories made in this test
mkdir ../tse-index-output

make

mkdir  ../tse-index-output/argstest-index


echo "Integration testing for indexer module."
echo "PLEASE ensure you have tse-output downloading before running the below script."
echo "For details on how to download tse-output, please see the general README."
echo "Currently using -DTEST flag to see progress indicators (e.g. current file ID)."
echo "(Please comment out for brevity, see Makefile in common to delete the flag)"
echo "Note, using -qy tags for diff, meaning that only if there are differences will they be displayed"
echo "Please use valgrind to check for memory leaks"

echo "========================================================="

#************************************* various commandline arguments testing block ************************************#
### The tests cases defined in this block should  all fail ###
echo "Section 0: Testing: Parameter Checks"

echo "Testing indexer (error-handling)"

# 2 arguments
echo "Error handling: Insufficient arguments"
./indexer ../tse-output/letters-depth-0
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Insufficient arguments provided"

fi

# 4 arguments
echo "Error handling: Too many arguments"
./indexer ../tse-output/letters-depth-0 ../tse-index-output/argstest-index ../tse-output/
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Too many arguments provided"

fi

# invalid pagedirectory (not crawler-produced)
echo "Error handling: Testing indexer on invalid pagedirectory (not crawler-produced)"
./indexer ../tse-index-output/argstest-index ../tse-index-output/argstest-index-out
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Indexer requires a crawler-produced directory"

fi

# invalid pagedirectory (non-existent)
echo "Error-handling: Testing crawler on invalid pagedirectory (non-existent)"
./indexer ../tse-output-not-exist/ ../tse-index-output/argstest-index-out
if [ $? -eq 0 ]; then
    echo >&2 "Error caught: Indexer ignores a non-existent pagedirectory"

fi

# invalid directory (no write permission)
mkdir ../tse-index-output/notWriteable
chmod -w ../tse-index-output/notWriteable

echo "Error-handling: Testing indexer on invalid directory (no write permission)"
./indexer ../tse-output/letters-depth-0 ../tse-index-output/notWriteable/letters-index-0
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Directory must be writable"

fi

chmod +w ../tse-index-output/notWriteable

#*****************************************Regular(valid) cases testing block **********************************#
echo "============================================================"

# The tests cases defined in this block should  all pass ###
echo " Section 1 Testing: Indexing letters, toscrape and wikipedia crawler produced directories  "

echo "Testing indexer on letters at depth 0, 1, 2, 3, 4, 5, 6"



# indexer on letters at depth 0
echo "Testing indexer on letters at depth 0 file"
./indexer ../tse-output/letters-depth-0 ../tse-index-output/letters-index-0
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on letters at depth 0 failed"
    exit 1
fi



# indexer on letters at depth 1
echo "Testing indexer on letters at depth 1 file"
./indexer ../tse-output/letters-depth-1 ../tse-index-output/letters-index-1
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on letters at depth 1 failed"
    exit 1
fi




# indexer on letters at depth 2
echo "Testing indexer on letters at depth 2 file"
./indexer ../tse-output/letters-depth-2/ ../tse-index-output/letters-index-2
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on letters at depth 2 failed"
    exit 1
fi




# indexer on letters at depth 3
echo "Testing indexer on letters at depth 3 file"
./indexer ../tse-output/letters-depth-3/ ../tse-index-output/letters-index-3
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on letters at depth 3 failed"
    exit 1
fi




# indexer on letters at depth 4

echo "Testing on letters html at depth 4 (contains loops)"
./indexer ../tse-output/letters-depth-4/ ../tse-index-output/letters-index-4
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on letters html failed"
    exit 1
fi





# # indexer on letters at depth 5
# echo "Testing indexer on letters at depth 5 file"
# ./indexer ../tse-output/letters-depth-5/ ../tse-index-output/letters-index-5
# if [ $? -ne 0 ]; then
#     echo >&2 "Indexer on letters at depth 5 failed"
#     exit 1
# fi

# echo "Comparing differences with tse-output"
# diff -qy <(sort ../tse-output/letters-index-5) <(sort ../tse-index-output/letters-index-5)




echo "Manual verification of files -- no words are missing or duplicated"

echo "========================================================="

echo "Testing indexer on Wikipedia playground at depth 0, 1, 2"




# indexer on wikipedia  at depth 0
echo "Testing indexer on Wikipedia at depth 0 file"
./indexer ../tse-output/wikipedia-depth-0 ../tse-index-output/wikipedia-index-0
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on wikipedia at depth 0 failed"
    exit 1
fi




# indexer on wikipedia  at depth 1
echo "Testing indexer on Wikipedia at depth 1 file"
./indexer ../tse-output/wikipedia-depth-1/ ../tse-index-output/wikipedia-index-1
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on wikipedia at depth 1 failed"
    exit 1
fi





# indexer on wikipedia  at depth 2
echo "Testing indexer on Wikipedia at depth 2 file"
./indexer ../tse-output/wikipedia-depth-2/ ../tse-index-output/wikipedia-index-2
if [ $? -ne 0 ]; then
    echo >&2 "Indexer on wikipedia at depth 2 failed"
    exit 1
fi






echo "========================================================="
echo "Testing indexer on toscrape playground at depth 0, 1, 2"

# # indexer on toscrape  at depth 0
# echo "Testing indexer on toscrape at depth 0 file"
# ./indexer ../tse-output/toscrape-depth-0/ ../tse-index-output/toscrape-index-0
# if [ $? -ne 0 ]; then
#     echo >&2 "Indexer on toscrape at depth 0 failed"
#     exit 1
# fi

# echo "Comparing differences with tse-output"
# diff -qy <(sort ../tse-output/toscrape-index-0) <(sort ../tse-index-output/toscrape-index-0)

# # indexer on toscrape  at depth 1
# echo "Testing indexer on toscrape at depth 1 file"
# ./indexer ../tse-output/toscrape-depth-1/ ../tse-index-output/toscrape-index-1
# if [ $? -ne 0 ]; then
#     echo >&2 "Indexer on toscrape at depth 1 failed"
#     exit 1
# fi

# echo "Comparing differences with tse-output"
# diff -qy <(sort ../tse-output/toscrape-index-1) <(sort ../tse-index-output/toscrape-index-1)

# # indexer on toscrape  at depth 2
# echo "Testing indexer on toscrape at depth 2 file"
# ./indexer ../tse-output/toscrape-depth-2/ ../tse-index-output/toscrape-index-2
# if [ $? -ne 0 ]; then
#     echo >&2 "Indexer on toscrape at depth 2 failed"
#     exit 1
# fi

# echo "Comparing differences with tse-output"
# diff -qy <(sort ../tse-output/toscrape-index-2) <(sort ../tse-index-output/toscrape-index-2)






#/********************************** Indextest Testing block *****************************************/
echo "*******************************************************"
echo "************ Indextest Testing block ******************"

echo " Section 2: Testing indextest error handling "

echo "Note: only basic argument number tests were done as per  REQUIREMENTS"



# 2 arguments
echo "Error-handling: Insufficient arguments"
./indextest ../tse-index-output/letters-depth-1/
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Insufficient arguments provided"
fi



# 4 arguments
echo "Error-handling: Too many arguments"
./indextest ../tse-index-output/letters-depth-1/ ../tse-index/output/ ../tse-output/
if [ $? -ne 0 ]; then
    echo >&2 "Error caught: Too many arguments provided"
fi



#*********************** Regular(valid) cases testing block for indextest **********************#
echo "============================================================"
# The tests cases defined in this block should  all pass 
echo " Also comparing sorted files of indexer and indextest "


# include indexsort.awk to sort the files and creat the directory to hold those files
chmod +x indexsort.awk
mkdir ../tse-sorted-index
mkdir ../tse-indextest-output


echo "Testing indextest on letters at depth 0, 1, 2, 3, 4, 5"
echo "Note: These are simple closed sets of cross-linked web pages"




# indextest on letters at depth 0
echo "Testing indextest on letters at depth 0 file"
./indextest ../tse-index-output/letters-index-0 ../tse-indextest-output/letters-index-0
if [ $? -ne 0 ]; then
    echo >&2 "Indextest on letters at depth 0 failed"
    exit 1
fi

# "Comparing differences between indextest sorted output and that of with indexer sorted "
echo "Comparing differences between indextest sorted output and that of with indexer sorted "
./indexsort.awk ../tse-index-output/letters-index-0 > ../tse-index-output/letters-index-0-sorted
./indexsort.awk ../tse-indextest-output/letters-index-0 > ../tse-sorted-index/letters-index-0-sorted

diff -qy ../tse-index-output/letters-index-0-sorted ../tse-sorted-index/letters-index-0-sorted
if [ $? -ne 0 ]; then
    echo >&2 "letters depth 0 indexer and indextest files are different "
    echo " "
    exit 1
fi




# indextest on letters at depth 1
echo "Testing indextest on letters at depth 1 file"
./indextest ../tse-index-output/letters-index-1 ../tse-indextest-output/letters-index-1
if [ $? -ne 0 ]; then
    echo >&2 " Indextest on letters at depth 1 failed"
    exit 1
fi

# Comparing differences between indextest sorted output and that of with indexer sorted "
echo "Comparing differences between indextest sorted output and that of with indexer sorted "
./indexsort.awk ../tse-index-output/letters-index-1 > ../tse-index-output/letters-index-1-sorted
./indexsort.awk ../tse-indextest-output/letters-index-1 > ../tse-sorted-index/letters-index-1-sorted

diff -qy ../tse-index-output/letters-index-1-sorted ../tse-sorted-index/letters-index-1-sorted
if [ $? -ne 0 ]; then
    echo >&2 "letters depth 1 indexer and indextest files are different "
    echo " "
    exit 1
fi





# indextest on letters at depth 2
echo "Testing indextest on letters at depth 2 file"
./indextest ../tse-index-output/letters-index-2 ../tse-indextest-output/letters-index-2
if [ $? -ne 0 ]; then
    echo >&2 "Indextest on letters at depth 2 failed"
    echo " "
    exit 1
fi

# Comparing differences between indextest sorted output and that of with indexer sorted "
echo "Comparing differences between indextest sorted output and that of with indexer sorted "
./indexsort.awk ../tse-index-output/letters-index-2 > ../tse-index-output/letters-index-2-sorted
./indexsort.awk ../tse-indextest-output/letters-index-2 > ../tse-sorted-index/letters-index-2-sorted

diff -qy ../tse-index-output/letters-index-2-sorted ../tse-sorted-index/letters-index-2-sorted
if [ $? -ne 0 ]; then
    echo >&2 "letters depth 2 indexer and indextest files are different "
    exit 1
fi





# indextest on letters at depth 3
echo "Testing indextest on letters at depth 3 file"
./indextest ../tse-index-output/letters-index-3 ../tse-indextest-output/letters-index-3
if [ $? -ne 0 ]; then
    echo >&2 "Indextest on letters at depth 3 failed"
    exit 1
fi

# Comparing differences between indextest sorted output and that of with indexer sorted "
echo "Comparing differences between indextest sorted output and that of with indexer sorted "
./indexsort.awk ../tse-index-output/letters-index-3 > ../tse-index-output/letters-index-3-sorted
./indexsort.awk ../tse-indextest-output/letters-index-3 > ../tse-sorted-index/letters-index-3-sorted

diff -qy ../tse-index-output/letters-index-3-sorted ../tse-sorted-index/letters-index-3-sorted
if [ $? -ne 0 ]; then
    echo >&2 "letters depth 3 indexer and indextest files are different "
    echo " "
    exit 1
fi




# indextest on letters at depth 4
echo "Testing indextest on letters at depth 4 file"
./indextest ../tse-index-output/letters-index-4 ../tse-indextest-output/letters-index-4
if [ $? -ne 0 ]; then
    echo >&2 "Indextest on letters at depth 4 failed"
     echo " "
    exit 1
fi

# Comparing differences between indextest sorted output and that of with indexer sorted "
echo "Comparing differences between indextest sorted output and that of with indexer sorted "
./indexsort.awk ../tse-index-output/letters-index-4 > ../tse-index-output/letters-index-4-sorted
./indexsort.awk ../tse-indextest-output/letters-index-4 > ../tse-sorted-index/letters-index-4-sorted

diff -qy ../tse-index-output/letters-index-4-sorted ../tse-sorted-index/letters-index-4-sorted
if [ $? -ne 0 ]; then
    echo >&2 "letters depth 4 indexer and indextest files are different "
     echo " "
    exit 1
fi




# # indextest on letters at depth 5
# echo "Testing indextest on letters at depth 5 file"
# ./indextest ../tse-index-output
# /letters-index-5 ../tse-indextest-output/letters-index-5
# if [ $? -ne 0 ]; then
#     echo >&2 "Indextest on letters at depth 5 failed"
#     exit 1
# fi

# echo "Comparing differences with tse-output"
# diff -qy <(sort ../tse-output/letters-index-5) <(sort ../tse-indextest-output/letters-index-5)




echo "Manual verification of files -- no words are missing or duplicated"

echo "==============================================================="
echo "Testing indextest on Wikipedia playground at depth 0, 1, 2"


# indextest on wikipedia at depth 0
echo "Testing indextest on Wikipedia at depth 0 file"
./indextest ../tse-index-output/wikipedia-index-0 ../tse-indextest-output/wikipedia-index-0
if [ $? -ne 0 ]; then
    echo >&2 "indextest on wikipedia at depth 0 failed"
     echo " "
    exit 1
fi

# Comparing differences between indextest sorted output and that of with indexer sorted "
echo "Comparing differences between indextest sorted output and that of with indexer sorted "
# sort both files
./indexsort.awk ../tse-index-output/wikipedia-index-0 > ../tse-index-output/wikipedia-index-0-sorted
./indexsort.awk ../tse-indextest-output/wikipedia-index-0 > ../tse-sorted-index/wikipedia-index-0-sorted

# compare
diff -qy ../tse-index-output/wikipedia-index-0-sorted ../tse-sorted-index/wikipedia-index-0-sorted
if [ $? -ne 0 ]; then
    echo >&2 "wikipedia depth 0 indexer and indextest files are different "
    exit 1
fi




# indextest on wikipedia at depth 1
echo "Testing indextest on Wikipedia at depth 1 file"
./indextest ../tse-index-output/wikipedia-index-1 ../tse-indextest-output/wikipedia-index-1
if [ $? -ne 0 ]; then
    echo >&2 "indextest on wikipedia at depth 1 failed"
    exit 1
fi

# Comparing differences between indextest sorted output and that of with indexer sorted "
echo "Comparing differences between indextest sorted output and that of with indexer sorted "
# sort both files
./indexsort.awk ../tse-index-output/wikipedia-index-1 > ../tse-index-output/wikipedia-index-1-sorted
./indexsort.awk ../tse-indextest-output/wikipedia-index-1 > ../tse-sorted-index/wikipedia-index-1-sorted

# compare
diff -qy ../tse-index-output/wikipedia-index-1-sorted ../tse-sorted-index/wikipedia-index-1-sorted
if [ $? -ne 0 ]; then
    echo >&2 "wikipedia depth 1 indexer and indextest files are different "
    exit 1
fi




# indextest on wikipedia at depth 2
echo "Testing indextest on Wikipedia at depth 2 file"
./indextest ../tse-index-output/wikipedia-index-2 ../tse-indextest-output/wikipedia-index-2
if [ $? -ne 0 ]; then
    echo >&2 "indextest on wikipedia at depth 2 failed"
    exit 1
fi

# Comparing differences between indextest sorted output and that of with indexer sorted "
echo "Comparing differences between indextest sorted output and that of with indexer sorted "
# sort both files
./indexsort.awk ../tse-index-output/wikipedia-index-2 > ../tse-index-output/wikipedia-index-2-sorted
./indexsort.awk ../tse-indextest-output/wikipedia-index-2 > ../tse-sorted-index/wikipedia-index-2-sorted

# compare 
diff -qy ../tse-index-output/wikipedia-index-2-sorted ../tse-sorted-index/wikipedia-index-2-sorted
if [ $? -ne 0 ]; then
    echo >&2 "wikipedia depth 2 indexer and indextest files are different "
    exit 1
fi

echo "=============================================================="
echo "Testing indextest on toscrape playground at depth 0, 1, 2"

# # indextest on toscrape at depth 0
# echo "Testing indextest on toscrape at depth 0 file"
# ./indextest ../tse-index-output/toscrape-index-0 ../tse-indextest-output/toscrape-index-0
# if [ $? -ne 0 ]; then
#     echo >&2 "indextest on toscrape at depth 0 failed"
#     exit 1
# fi

# echo "Comparing differences with tse-output"
# diff -qy <(sort ../tse-index-output/toscrape-index-0) <(sort ../tse-indextest-output/toscrape-index-0)

# # indextest on toscrape at depth 1
# echo "Testing indextest on toscrape at depth 1 file"
# ./indextest ../tse-index-output/toscrape-index-1 ../tse-indextest-output/toscrape-index-1
# if [ $? -ne 0 ]; then
#     echo >&2 "indextest on toscrape at depth 1 failed"
#     exit 1
# fi

# echo "Comparing differences with tse-output"
# diff -qy <(sort ../tse-index-output
# /toscrape-index-1) <(sort ../tse-indextest-output/toscrape-index-1)

# # indextest on toscrape at depth 2
# echo "Testing indextest on toscrape at depth 2 file"
# ./indextest ../tse-index-output/toscrape-index-2 ../tse-indextest-output/toscrape-index-2
# if [ $? -ne 0 ]; then
#     echo >&2 "indextest on toscrape at depth 2 failed"
#     exit 1
# fi

# echo "Comparing differences with tse-output"
# diff -qy <(sort ../tse-index-output/toscrape-index-2) <(sort ../tse-indextest-output/toscrape-index-2)

echo "================== End of testing.sh =================="

exit 0

