#ifndef SET_OPTIMIST_LOCKING_H
#define SET_OPTIMIST_LOCKING_H


#include <pthread.h>


/* Set of integers 
   - element e of a set must be such that  INT_MIN < e < INT_MAX
   - a set is represented by an ordered linked list of nodes 
*/


typedef struct node
{
    int key;
    struct node* next;
    pthread_mutex_t mutex;
} node_t;

typedef struct intset
{
    node_t* head;
} intset_t;


/* Insert val into set
   return 0 if val already present
   1 otherwise */
int set_add(intset_t* set, int val);


/* Search val in set
   return 1 if val is present
   0 otherwise */
int set_contain(intset_t* set, int val);


/* Create an empty set */
int set_new(intset_t* set);


/* remove val from set
   return 1 if val is present
   0 otherwise */
int set_remove(intset_t* set, int val);


/* Print set 
   for debugging purpose only
   do not have to be made concurrent  */
void set_print(intset_t* set);


#endif // SET_OPTIMIST_LOCKING_H
