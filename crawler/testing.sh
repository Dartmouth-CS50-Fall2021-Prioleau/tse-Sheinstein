#!/bin/bash
#
# Testing script for crawler.c
# 
# Author: Salifyanji J Namwila October, 12 2021
# CS50 Fall 2021.
# Based on testing script by Temi Prioleau
# Feb 5, 2020

# USAGE: bash -v testing.sh
#ARGUMENTS: ./crawler seedURL pageDirectory maxDepth
#example: ./crawler http://cs50tse.cs.dartmouth.edu/tse/ ./data/ 2

#******************* MAKE *.OUT FILE EXECUTABLE && DEFINE `GLOBAL` VARS**************************#

chmod +x testing.sh # change mode of .out file to executable


# Define variables
externalURL = http://www.google.com/
seedURL = http://cs50tse.cs.dartmouth.edu/tse/letters/

echo "Initializing Test ..."

#************************************* various commandline arguments testing block ************************************#
### The tests cases defined in this block should  all fail ###
#NB:  All returns from ./crawler that are arguments centered and unsuccessful returned with status 1

# make directory to hold all the testing output directories
#mkdir tse-output

# make directories within tse-output
mkdir depth-test # make new directory to where document_ID files will be written
mkdir letters-depth-0
mkdir letters-depth-1
mkdir letters-depth-2
mkdir letters-depth-3
mkdir letters-depth-4
mkdir letters-depth-5

mkdir toscrape-depth-0
mkdir toscrape-depth-1
mkdir dartmouth-depth-2

mkdir wikipedia-depth-0
mkdir wikipedia-depth-1


# 1 argument 

echo " "
./crawler
if [ $? -eq 0 ]
then
	echo "Passed test with 1 argument"
    echo " "
else
	echo "Failed test with 1 argument"
    echo " "
fi

# 2 arguments
./crawler $seedURL
if [ $? -eq 0 ]
then
	echo "Passed test w/ 2 arguments"
    echo " "
else
	echo "Failed test w/ 2 arguments"
    echo " "
fi

# 3 arguments
./crawler $seedURL depth-test
if [ $? -eq 0 ]
then
	echo "Passed test w/ 3 arguments"
    echo " "
else
	echo "Failed test w/ 3 arguments"
    echo " "
fi

# 4 arguments + externalURL
./crawler $externalURL depth-test 2
if [ $? -eq 0 ]
then
	echo "Passed test w/ external URL"
    echo " "
else
	echo "Failed test w/ external URL"
    echo " "
fi

# 4 arguments + non-existent server
#./crawler http://sheinstein.com/notyetexist.php argstest 2
#if [ $? -eq 0 ]
#then
#	echo "Passed test w/ non-existent server"
#    echo " "
#else
#	echo "Failed test w/ non-existent server"
#    echo " "
#fi

# 4 arguments + valid server, non-existent page
#./crawler http://cs50tse.cs.dartmouth.edu/tse/salie argstest 2 # URL from Marlee Montella
#if [ $? -eq 0 ]
#then
#	echo "Passed test w/ valid server, non-existent page"
#    echo " "
#else
	echo "Failed test w/ valid server, non-existent page"
    echo " "
#fi

# 4 arguments + negative page depth
./crawler $seedURL depth-test -10
if [ $? -eq 0 ]
then
	echo "Passed test w/ negative page depth"
    echo " "
else
	echo "Failed test w/ negative page depth"
    echo " "
fi

echo " "

#*********************************************Regular(valid) cases testing block **************************************#
### The tests cases defined in this block should  all pass ###

## Crawling a simple closed set of cross-linked web pages;should not produce duplicate  or missing files.

# at depth 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html letters-depth-0 0
if [ $? -eq 0 ]
then
    if [ -f ./letters-depth-0/1 ] 
    then
        echo "Passed test at depth = 0"
        echo " "
    else
        echo "Failed test at depth = 0"
        echo " "
	fi
else
	echo "Failed test at depth = 0"
    echo " "
fi

# at depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html letters-depth-1 1
if [ $? -eq 0 ]
then
    if [ -f ./letters-depth-1/2 ]
    then
        echo "Passed test at depth = 1"
        echo " "
    else
        echo "Failed test at depth = 1"
        echo " "
	fi
else
	echo "Failed test at depth = 1"
    echo " "
fi

# at depth 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html letters-depth-2 2
if [ $? -eq 0 ]
then
    if [ -f ./letters-depth-2/3 ]
    then
        echo "Passed test at depth = 2"
        echo " "
    else
        echo "Failed test at depth = 2"
        echo " "
	fi
else
	echo "Failed test at depth = 2"
    echo " "
fi

# at depth 3
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html letters-depth-3 3
if [ $? -eq 0 ]
then
    if [ -f ./letters-depth-3/6 ]
    then
        echo "Passed test at depth = 3"
        echo " "
    else
        echo "Failed test at depth = 3"
        echo " "
	fi
else
	echo "Failed test at depth = 3"
    echo " "
fi

# at depth 4

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./letters-depth-4/ 4 
if [ $? -eq 0 ]
then
    if [ -f ./letters-depth-4/8 ]
    then
        echo "Passed test at depth = 4"
        echo " "
    else
        echo "Failed test at depth = 4"
        echo " "
	fi
else
	echo "Failed test at depth = 4"
    echo " "
fi

# at depth 5
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./letters-depth-5/ 5
if [ $? -eq 0 ]
then
    if [ -f ./letters-depth-5/9 ]
    then
        echo "Passed test at depth = 5"
        echo " "
    else
        echo "Failed test at depth = 5"
        echo " "
	fi
else
	echo "Failed test at depth = 5"
    echo " "
fi


## Repeating previous tests with a different seed page in the same site
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ./toscrape-depth-0/ 0
if [ $? -eq 0 ]
then
	if [ -f ./toscrape-depth-0/1 ]
	then
		echo "Passed test w/ closed set of cross-linked pages, different seed page"
        echo " "
	else
		echo "Failed test w/ closed set of cross-linked pages, different seed page"
        echo " "
	fi
else
    echo "Failed test w/ closed set of cross-linked pages, different seed page"
    echo " "
fi

# at depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ./toscrape-depth-1/ 1
if [ $? -eq 0 ]
then
	if  [ -f ./toscrape-depth-1/73 ]
	then
		echo "Passed test w/ closed set of cross-linked pages, different seed page"
        echo " "
	else
		echo "Failed test w/ closed set of cross-linked pages, different seed page"
        echo " "
	fi
else
    echo "Failed test w/ closed set of cross-linked pages, different seed page"
    echo " "
fi
# at depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ./toscrape-depth-1/ 1
if [ $? -eq 0 ]
then
	if  [ -f ./toscrape-depth-1/73 ]
	then
		echo "Passed test w/ closed set of cross-linked pages, different seed page"
        echo " "
	else
		echo "Failed test w/ closed set of cross-linked pages, different seed page"
        echo " "
	fi
else
    echo "Failed test w/ closed set of cross-linked pages, different seed page"
    echo " "
fi

#  at depth 2 with cross linked cycles
./crawler http://cs50tse.cs.dartmouth.edu/index.html dartmouth-depth-2 2
if [ $? -eq 0 ]
then
 	if  [ -f dartmouth-depth-2/4]
 	then
 		echo "Passed test w/ closed set of cross-linked pages, different seed page"
        echo " "
 	else
 		echo "Failed test w/ closed set of cross-linked pages, different seed page"
        echo " "
 	fi
else
    echo "Failed test w/ closed set of cross-linked pages, different seed page"
    echo " "
 fi



## Crawling Wikipedia playground. Exploring at depths 0, 1, and 2


# at depth 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ./wikipedia-depth-0/ 0
if [ $? -eq 0 ]
then
    if [ -f ./wikipedia-depth-0/1 ]
    then
        echo "Passed wiki test at depth = 0"
        echo " "
    else
        echo "Failed wiki test at depth = 0"
        echo " "
	fi
else
	echo "Failed wiki test at depth = 0"
    echo " "
fi

# at depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ./wikipedia-depth-1/ 1
if [ $? -eq 0 ]
then
    if [ -f ./wikipedia-depth-1/7 ]
    then
        echo "Passed wiki test at depth = 1"
        echo " "
    else
        echo "Failed wiki test at depth = 1"
        echo " "
	fi
else
	echo "Failed wiki test at depth = 1"
    echo " "
fi

# # at depth 2
# mkdir wikipedia-depth-2
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ./wikipedia-depth-2/ 2
# if [ $? -eq 0 ]
# then
#     if [ -f wikipedia-depth-2/999]#[[ $(ls ./wikipedia-depth-2/* | wc -l) -eq  999]]]
#     then
#         echo "Passed wiki test at depth = 1"
#         echo " "
#     else
#         echo "Failed wiki test at depth = 1"
#         echo " "
# 	fi
# else
# 	echo "Failed wiki test at depth = 1"
#     echo " "
# fi



echo " Testing Complete."
echo " "

