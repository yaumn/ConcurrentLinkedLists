#include <stdio.h>


#if defined(ONE_LOCK)
# include "set_one_lock.h"
#elif defined(LOCK_COUPLING)
# include "set_lock_coupling.h"
#elif defined(OPTIMISTIC_LOCKING)
# include "set_optimistic_locking.h"
#elif defined(LAZY_LOCKING)
# include "set_lazy_locking.h"
#elif defined(LOCK_FREE)
# include "set_lock_free.h"
#endif


#ifndef THREAD_NB
# define THREAD_NB 5
#endif


void *test(void *a)
{
    intset_t *set = a;
    
    fprintf(stderr, "main: %d\n", set->head->key);
    fprintf(stderr, "main: %d\n", set->head->next->key);

    fprintf(stderr, "contain 3 %s\n set = ", set_contain(set, 3) ? "OK" : "NOK");

    fprintf(stderr,"add 0, 4, 2 \nset = ");
    set_add(set, 0);
    set_add(set, 4);
    set_add(set, 2);

    fprintf(stderr, "remove 4 %s\n set = ", set_remove(set, 4) ? "OK" : "NOK");
    fprintf(stderr, "remove 3 %s\n set = ", set_remove(set, 3) ? "OK" : "NOK");
    fprintf(stderr, "contain 3 %s\n set = ", set_contain(set, 3) ? "OK" : "NOK");
    fprintf(stderr, "contain 2 %s\n set = ", set_contain(set, 2) ? "OK" : "NOK");

    return a;
}


int main()
{
    intset_t set;
    set_new(&set);

    pthread_t threads[THREAD_NB];
    for (int i = 0 ; i < THREAD_NB ; i++) {
	pthread_create(&threads[i], NULL, test, &set);
    }

    for (int i = 0 ; i < THREAD_NB ; i++) {
	pthread_join(threads[i], NULL);
    }
    
    set_print(&set);

    return 0;
}
