/* 
 * word.c - CS50 'word' module
 *
 * see word.h for more information.
 *
 * Salifyanji J Namwila October 19, 2021.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word.h"
#include <ctype.h>

/**************** functions ****************/

/**************** normalize_word ****************/
char* normalize_word(char* word)
{
    for(int i = 0; i <= (strlen(word)); i++) {
        word[i] = tolower(word[i]);
    }

    return word;
}

