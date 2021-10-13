/* 
 * hashtable.c - CS50 'hashtable' module
 *
 * see hashtable.h for more information.
 *
  * Salifyanji Namwila, Oct 02, 2021 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"
#include "memory.h"
#include "jhash.h"
#include "set.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct hashtablenode {
  void *item;		      // pointer to data for this item
  const char *key;              // string key to this item
  struct hashtablenode *next;	      // link to next node
} hashtablenode_t;

/**************** global types ****************/
typedef struct hashtable {
  set_t **array;	      // array of pointers to sets
  unsigned long size;
} hashtable_t;

/**************** global functions ****************/
/* that is, visible outside this file */

/**************** local functions ****************/
/* not visible outside this file */

/**************** hashtable_new() ****************/
hashtable_t *hashtable_new(const int num_slots)
{
    hashtable_t *hashtable = count_malloc(sizeof(hashtable_t));
    if ( hashtable == NULL) {
        return NULL; //error allocating hashtable
    } else {
        // initialize array of set pointers
        hashtable->size = num_slots;
        hashtable->array = count_calloc(num_slots, sizeof(set_t*));
        for (int i = 0; i < num_slots; i ++){
            hashtable->array[i] = set_new();
        }
        return hashtable;
    }
}

/**************** hashtable_insert() ****************/
    bool hashtable_insert(hashtable_t *ht, const char *key, void *item)
    {
        if (ht != NULL && item != NULL && key != NULL) {
            unsigned long hash_number = JenkinsHash(key, ht->size);
            return set_insert(ht->array[hash_number], key, item);
        }
        return false;
    }

/**************** hashtable_find() ****************/
    void *hashtable_find(hashtable_t *ht, const char *key){
        if (ht == NULL || key == NULL){
            return NULL;
        }
        unsigned long hash_number = JenkinsHash(key, ht->size);
        return set_find(ht->array[hash_number], key);
    }

/**************** hashtable_print() ****************/
    void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item))
    {
        if (fp == NULL){
            return;
        }
        if (ht == NULL){
            printf("(null)");
            return;
        }
        for (int i = 0; i < ht->size; i++){
            printf("[%d]", i);
            set_print(ht->array[i], fp, (*itemprint));
        }
    }

/**************** hashtable_iterate() ****************/
    void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) )
    {
        if (ht == NULL || itemfunc == NULL){
            return;
        }
         for (int i = 0; i < ht->size; i++){
            set_iterate(ht->array[i], arg, (*itemfunc));
        }
    }

    /**************** hashtable_delete() ****************/
    void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) )
    {
        if (ht != NULL) {
            for (int i = 0; i < ht->size; i++){
                set_delete(ht->array[i], (*itemdelete));
            }
            count_free(ht->array);
            count_free(ht);
        }
        #ifdef MEMTEST
            count_report(stdout, "End of hashtable_delete");
        #endif
    }

