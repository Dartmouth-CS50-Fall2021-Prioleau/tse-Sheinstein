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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word.h"
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

/*********************** normalize_word() *************************/
/* Coverts a given word to all-lower case and returns the  word to caller
 * Input: a string , char*, word 
 * Output: lower-case version of the  same word
 * return null toif word is NULL
 */
char* normalize_word(char* word); 


/***************************** is_alpha_tokens() *******************************/
/* Extracts words from a line of words and returns the number of extracted words
 * Extracts by delimeter " ", adds word to given array if word
 * comprises of only letters.
 * brekas if a word contains non-alpha character.
 */
bool
is_alpha_tokens(char* line, const char* arr[], int* address);



/*************************** is_Alpha() ***************************/
/* Returns true if given character contains only letters
 * Returns false otherwise.
*/
bool is_Alpha(char *word);


#endif // __WORD_H

