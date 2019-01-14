#include <limits.h>  
#include <stdio.h>
#include <stdlib.h>

#include "set_one_lock.h"


int set_new(intset_t *set)
{
    /* sentinels */
    node_t *head = malloc(sizeof(node_t));
    node_t *tail = malloc(sizeof(node_t));
    
    head->key = INT_MIN;
    head->next = tail;
    tail->key = INT_MAX;
    tail->next = NULL;
    set->head = head;

    pthread_mutex_init(&set->mutex, NULL);
    
    return 1; 
}


int set_contain(intset_t *set, int val)
{
    pthread_mutex_lock(&set->mutex);
    
    node_t *curr;
    node_t *pred;

    pred = set->head;
    curr = pred->next;

    while (curr->key < val) {
	pred = curr;
	curr = pred->next;
    }

    const int success = curr->key == val;

    pthread_mutex_unlock(&set->mutex);
    
    return success;
}


int set_add(intset_t *set, int val)
{
    pthread_mutex_lock(&set->mutex);
    
    node_t *curr;
    node_t *pred;

    pred = set->head;
    curr = pred->next;
    
    while (curr->key < val) {
	pred = curr;
	curr = pred->next;
    }

    int success = 0;
    
    if (curr->key != val) {	
	node_t *new = malloc(sizeof(node_t));
	new->key = val;
	new->next = curr;
	pred->next = new;
	success = 1;
    }
    
    pthread_mutex_unlock(&set->mutex);
    
    return success;
}


int set_remove(intset_t *set, int val)
{
    pthread_mutex_lock(&set->mutex);
    
    node_t *curr;
    node_t *pred;

    pred = set->head;
    curr = pred->next;

    while (curr->key < val) {
	pred = curr;
	curr = pred->next;
    }

    int success = 0;
    
    if (curr->key == val) {
	pred->next = curr->next;
	success = 1;
    }
    
    pthread_mutex_unlock(&set->mutex);
    
    return success;
}


void set_print(intset_t *set)
{
    node_t *curr;
    node_t *pred;

    pred = set->head;
    curr = pred->next;

    while (curr->key < INT_MAX) {
	printf("%d ", curr->key);
	pred = curr;
	curr = pred->next;
    }
    printf("\n");
}
