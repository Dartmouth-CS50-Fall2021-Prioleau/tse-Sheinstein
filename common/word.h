/**
 * word.h -- header file for CS50 'word' module
 * 
 * 'word' module  is a collection of methods utilized by among 
 * indexer indexer test and querier.
 * The module at the time of implemetion supports one 
 * method , `normalize_word`, a method thatchanges a 
 * given word lower case.
 * 
 * Salifyanji J Namwila
 * October  19 2021
 */

#ifndef __WORD_H
#define __WORD_H

#include <ctype.h>
#include <string.h>

/*********************** normalize_word() *************************/
/* Coverts a given word to all-lower case and returns the  word to caller
 * Input: a string , char*, word 
 * Output: lower-case version of the  same word
 * return null toif word is NULL
 */
char* normalize_word(char* word); 

#endif // __WORD_H
