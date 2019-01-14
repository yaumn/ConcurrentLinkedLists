#include <limits.h>  
#include <stdio.h>
#include <stdlib.h>

#include "set_lazy_locking.h"


int validate(node_t *pred, node_t *curr)
{
    return (!pred->marked && !curr->marked && pred->next == curr);
}


int set_new(intset_t *set)
{
    /* sentinels */
    node_t *head = malloc(sizeof(node_t));
    node_t *tail = malloc(sizeof(node_t));
   
    head->key = INT_MIN;
    head->next = tail;
    pthread_mutex_init(&head->mutex, NULL);
    head->marked = 0;
    tail->key = INT_MAX;
    tail->next = NULL;
    pthread_mutex_init(&tail->mutex, NULL);
    tail->marked = 0;
    set->head = head;

    return 1; 
}


int set_contain(intset_t *set, int val)
{
    node_t *curr;

    curr = set->head;
	
    while (curr->key < val) {
        curr = curr->next;
    }

    return curr->key == val && !curr->marked;
}


int set_add(intset_t *set, int val)
{
    node_t *curr;
    node_t *pred;

    while (1) {
	pred = set->head;
	curr = pred->next;

	while (curr->key < val) {
	    pred = curr;
	    curr = pred->next;
	}

	pthread_mutex_lock(&pred->mutex);
	pthread_mutex_lock(&curr->mutex);

	if (validate(pred, curr)) {
	    int success = 0;
	    if (curr->key != val) {
		node_t *new = malloc(sizeof(node_t));
		new->key = val;
		pthread_mutex_init(&new->mutex, NULL);
		new->marked = 0;
		new->next = curr;
		pred->next = new;
		success = 1;	
	    }
	    
	    pthread_mutex_unlock(&curr->mutex);
	    pthread_mutex_unlock(&pred->mutex);

	    return success;
	}
    
	pthread_mutex_unlock(&curr->mutex);
	pthread_mutex_unlock(&pred->mutex);
    }
}


int set_remove(intset_t *set, int val)
{
    node_t *curr;
    node_t *pred;

    while (1) {
	pred = set->head;
	curr = pred->next;
    
	while (curr->key < val) {
	    pred = curr;
	    curr = pred->next;
	}

	pthread_mutex_lock(&pred->mutex);
	pthread_mutex_lock(&curr->mutex);
	
	if (validate(pred, curr)) {
	    int success = 0;
	    if (curr->key == val) {
		curr->marked = 1;
		pred->next = curr->next;
		success = 1;
	    }
	    
	    pthread_mutex_unlock(&curr->mutex);
	    pthread_mutex_unlock(&pred->mutex);

	    return success;
	}
    
	pthread_mutex_unlock(&curr->mutex);
	pthread_mutex_unlock(&pred->mutex);
    }
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
