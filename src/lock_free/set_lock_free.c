#include <limits.h>  
#include <stdatomic.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "set_lock_free.h"


/** 
    The three functions below use the fact that on most platform,
    at least the least significant bit on an adress is not used
    (due to alignment). We can use this bit as the mark indicating
    the node was removed.
**/

int get_mark(void *ptr)
{
    return (uintptr_t)ptr & 1;
}


void *mark(void *ptr)
{
    return (void *)((uintptr_t)ptr | 1);
}


void *unmark(void *ptr)
{
    return (void *)((uintptr_t)ptr & ~1);
}


void find(node_t *head, int x, node_t **a, node_t **b)
{
    node_t *curr;
    node_t *pred;
    node_t *succ;

 retry:
    while (1) {
        pred = head;
	curr = pred->next;

	while (1) {
	    succ = curr->next;

	    while (get_mark(succ)) {
	        if (!atomic_compare_exchange_weak(&pred->next, &curr, (node_t *)unmark(succ))) {
		    goto retry;
		}
		curr = unmark(succ);
		succ = curr->next;
	    }

	    if (curr->key >= x) {
		*a = pred;
		*b = curr;
		return;
	    }

	    pred = curr;
	    curr = succ;
	}
    }
}


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

    return 1; 
}


int set_contain(intset_t *set, int val)
{
    node_t *curr = set->head;

    while (curr->key < val) {
	curr = unmark(curr->next);
    }

    return (curr->key == val && !get_mark(curr->next));
}


int set_add(intset_t *set, int val)
{
    node_t *curr;
    node_t *pred;
    node_t *new = malloc(sizeof(node_t));
	
    while (1) {
	find(set->head, val, &pred, &curr);
	if (curr->key == val) {
	    return 0;
	}

	new->key = val;
        new->next = curr;

	if (atomic_compare_exchange_weak(&pred->next, &curr, new)) {
	    return 1;
	}
    }
}


int set_remove(intset_t *set, int val)
{
    node_t *curr;
    node_t *pred;

    while (1) {
	find(set->head, val, &pred, &curr);
	if (curr->key != val) {
	    return 0;
	}

	node_t *succ = curr->next;
	if (atomic_compare_exchange_weak(&curr->next, &succ, (node_t *)mark(succ))) {
	    atomic_compare_exchange_weak(&pred->next, &curr, succ);
	    return 1;
	}
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
