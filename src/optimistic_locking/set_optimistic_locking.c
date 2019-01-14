#include <limits.h>  
#include <stdio.h>
#include <stdlib.h>

#include "set_optimistic_locking.h"


int validate(intset_t *set, node_t *pred, node_t *curr)
{
    node_t *it = set->head;
    while (it->key < pred->key) {
	it = it->next;
    }
    return (it->key == pred->key && it->next == curr);
}


int set_new(intset_t *set)
{
    /* sentinels */
    node_t *head = malloc(sizeof(node_t));
    node_t *tail = malloc(sizeof(node_t));
   
    head->key = INT_MIN;
    head->next = tail;
    pthread_mutex_init(&head->mutex, NULL);
    tail->key = INT_MAX;
    tail->next = NULL;
    pthread_mutex_init(&tail->mutex, NULL);
    set->head = head;

    return 1; 
}


int set_contain(intset_t *set, int val)
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
	
	if (validate(set, pred, curr)) {
	    const int success = curr->key == val;
	    
	    pthread_mutex_unlock(&curr->mutex);
	    pthread_mutex_unlock(&pred->mutex);
	    
	    return success;
	}
    
	pthread_mutex_unlock(&curr->mutex);
	pthread_mutex_unlock(&pred->mutex);
    }
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

	if (validate(set, pred, curr)) {
	    int success = 0;
	    
	    if (curr->key != val) {
		node_t *new = malloc(sizeof(node_t));
		new->key = val;
		pthread_mutex_init(&new->mutex, NULL);
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
	
	if (validate(set, pred, curr)) {
	    int success = 0;
	    if (curr->key == val) {
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
