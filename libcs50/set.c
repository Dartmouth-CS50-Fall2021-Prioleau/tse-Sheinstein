/* 
 * set.c - CS50 'set' module
 *
 * see set.h for more information.
 *
 * Salifyanji Namwila, Oct 02, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "set.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct setnode {
  void *item;		      // pointer to data for this item
  const char *key;              // string key to this item
  struct setnode *next;	      // link to next node
} setnode_t;

/**************** global types ****************/
typedef struct set {
  struct setnode *head;	      // head of the list of items in set
} set_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */
/**************** local functions ****************/

/**************** local functions ****************/
/* not visible outside this file */
static setnode_t *setnode_new(const char *key, void *item);

/**************** set_new() ****************/
set_t* set_new(void)
{
  set_t *set = count_malloc(sizeof(set_t));

  if (set == NULL) {
    return NULL; // error allocating set
  } else {
    // initialize contents of set structure
    set->head = NULL;
    return set;
  }
}

/**************** set_insert() ****************/
bool set_insert(set_t *set, const char *key, void *item)
{

  if (set != NULL && item != NULL && key != NULL) {
    // Check if key is in the set already
    for (setnode_t *node = set->head; node != NULL; node = node->next){
        if (strcmp(node->key, key) == 0){
            return false;
        }
    }
    //allocate memory and copy the key
    char *key_copy = count_malloc(sizeof(char) * strlen(key) + 1);
    strcpy(key_copy, key);

    // allocate a new node to be added to the list
    setnode_t *new = setnode_new(key_copy, item);
    if (new != NULL) {
      // add it to the head of the list
      new->next = set->head;
      set->head = new;
      return true;
    }
  }

  #ifdef MEMTEST
    count_report(stdout, "After set_insert");
  #endif
  return false;
}


/**************** setnode_new ****************/
/* Allocate and initialize a setnode */
// the 'static' modifier means this function is not visible 
// outside this file
static setnode_t* setnode_new(const char *key, void *item)
{
  setnode_t *node = count_malloc(sizeof(setnode_t));

  if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    node->item = item;
    node->key = key;
    node->next = NULL;
    return node;
  }
}

/**************** set_find() ****************/
void* set_find(set_t *set, const char *key)
{
  if (set == NULL || key == NULL) {
    return NULL; // bad set or bad key
  } else if (set->head == NULL) {
    return NULL; // set is empty
  } else {
    for (setnode_t *node = set->head; node != NULL; node = node->next) {
        if (strcmp(node->key, key) == 0){
            return node->item;
        }
    }
    return NULL;
  }
}

/**************** set_print() ****************/
void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item) )
{
  if (fp != NULL) {
    if (set != NULL) {
      fputc('{', fp);
      for (setnode_t *node = set->head; node != NULL; node = node->next) {
        // print this node
        if (itemprint != NULL) 
        {  // print the node's item 
          (*itemprint)(fp, node->key, node->item);
          if(node->next != NULL){
            fputc(',', fp);  
          }
         
        }
      }
      fputs(" }\n", fp);
    } else {
      fputs("(null)", fp);
      }
  }
}

/**************** set_iterate() ****************/

void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) )
{
    if (set != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (setnode_t *node = set->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->item); 
    }
  }
}

/**************** set_delete() ****************/
void set_delete(set_t *set, void (*itemdelete)(void *item) )
{
  if (set != NULL) {
    for (setnode_t* node = set->head; node != NULL; ) {
      if (itemdelete != NULL) {		    // if possible...
        (*itemdelete)(node->item);	  // delete node's item
      }
      count_free((char*)node->key);    // free the key
      setnode_t* next = node->next;	   // remember what comes next
      count_free(node);			 // free the node
      node = next;			    // and move on to next
    }
    count_free(set);          // free the set
  }

#ifdef MEMTEST
    count_report(stdout, "End of set_delete");
#endif
}
