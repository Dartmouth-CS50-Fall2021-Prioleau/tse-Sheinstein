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
#include <unistd.h>
#include <stdbool.h>

/**************** functions ****************/

/**************** normalize_word ****************/
char* normalize_word(char* word)
{
    for(int i = 0; i <= (strlen(word)); i++) {
        word[i] = tolower(word[i]);
    }

    return word;
}


/***************************** get_tokens() *******************************/
/* see words.h for description
 */

int 
get_tokens(char* line, const char* arr[])
 {

     int word_count = 0;

     //read queries from stdin
     char* word = strtok(line, " "); // extract words using " " delimeter
    
     while (word != NULL){
         if(is_Alpha(word)){
            arr[word_count] = word;
            //printf("extracted words is %s\n", word);
            word = strtok(NULL, " ");
            word_count++;
         }
         else break;
     }
     return word_count;
 }


/*************************** is_Alpha() ***************************/
/* Returns true if given character contains only letters
 * Returns false otherwise.
*/
bool is_Alpha(char *word) 
 {
     int i;
     for (i = 0; i < strlen(word); i++) {
         if (!isalpha(word[i])) { // check for bad characters
             fprintf(stderr, "Error: bad character '%c' in query.\n", word[i]);
             fprintf(stdout, "-----------------------------------------------------------------------------------\n");
             return false; 
         }
     }
     return true;
 }



 
