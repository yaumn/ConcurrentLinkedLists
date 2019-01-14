#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


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


char *operations = NULL;
volatile atomic_int operation_index = 0;
int max_value = 0;


void *perform_operation(void *set)
{
    int i;
    while (operations[(i = atomic_fetch_add(&operation_index, 1))] != '\0') {
	switch (operations[i]) {
	    case 'a':
		set_add(set, rand() % max_value);
		break;

	    case 'r':
		set_remove(set, rand() % max_value);
		break;

	    case 'c':
		set_contain(set, rand() % max_value);
		break;

	    default:
		break;
	}
    }
    return set;
}


void create_operations_array(const int nb_add, const int nb_remove, const int nb_contain)
{
    const int size = nb_add + nb_remove + nb_contain + 1;
    operations = malloc(size * sizeof(char));

    for (int i = 0 ; i < nb_add ; i++) {
	operations[i] = 'a';
    }
    for (int i = 0 ; i < nb_remove ; i++) {
	operations[nb_add + i] = 'r';
    }
    for (int i = 0 ; i < nb_contain ; i++) {
	operations[nb_add + nb_remove + i] = 'c';
    }
    operations[size - 1] = '\0';

    // Shuffle the array
    for (int i = 0 ; i < size - 1 ; i++) {
	const int random_index = rand() % (size - 1);
	const char tmp = operations[random_index];
	operations[random_index] = operations[i];
	operations[i] = tmp;
    }
}


double time_interval(const struct timespec *t1, const struct timespec *t2)
{
    double interval = 0.0;
    if (t1->tv_nsec > t2->tv_nsec) {
	interval = t2->tv_sec - t1->tv_sec - 1;
	interval += (t2->tv_nsec - t1->tv_nsec + 1000000000) / 1000000000.0;
    } else {
	interval = t2->tv_sec - t1->tv_sec;
	interval += (t2->tv_nsec - t1->tv_nsec) / 10000000000.0;
    }

    return interval;
}


int main(int argc, char *argv[])
{
    int nb_thread = 4;
    int nb_add = 500000;
    int nb_remove = 500000;
    int nb_contain = 1000000;
    int initial_set_size = 300;
    max_value = 4000;
    int seed = time(NULL);

    // Fetch arguments
    char c;
    while ((c = getopt(argc, argv, "a:c:i:m:r:s:t:")) != -1) {
	switch (c) {
	    case 'a':
		nb_add = atoi(optarg);
		break;
	    case 'c':
		nb_contain = atoi(optarg);
		break;
	    case 'i':
		initial_set_size = atoi(optarg);
		break;
	    case 'm':
		max_value = atoi(optarg);
		break;
	    case 'r':
		nb_remove = atoi(optarg);
		break;
	    case 's':
		seed = atoi(optarg);
		break;
	    case 't':
		nb_thread = atoi(optarg);
		break;
	    default:
		break;
	}
    }
    
    srand(seed);

    printf("Seed : %d\n", seed);
    printf("Number of threads : %d\n", nb_thread);
    printf("Number of add calls : %d\n", nb_add);
    printf("Number of remove calls : %d\n", nb_remove);
    printf("Number of contain calls : %d\n", nb_contain);
    printf("Initial set size : %d\n", initial_set_size);
    printf("Maximum value of set element : %d\n\n", max_value);
 
    intset_t set;
    set_new(&set);
    
    // Fill the set
    printf("Filling initial set...");
    int size = 0;
    while (size < initial_set_size) {
	size += set_add(&set, rand() % max_value);
    }
    puts("\tDone\n");
    
    create_operations_array(nb_add, nb_remove, nb_contain);

    pthread_t *threads = malloc(nb_thread * sizeof(pthread_t));

    struct timespec t1;
    timespec_get(&t1, TIME_UTC);
    
    for (int i = 0 ; i < nb_thread ; i++) {
	pthread_create(&threads[i], NULL, perform_operation, &set);
	printf("Thread %d created.\n", i);
    }

    printf("\n");
    printf("Performing operations...\n");

    for (int i = 0 ; i < nb_thread ; i++) {
	pthread_join(threads[i], NULL);
    }

    struct timespec t2;
    timespec_get(&t2, TIME_UTC);
    
    const double interval = time_interval(&t1, &t2);

    puts("All threads finished.\n");
    printf("Time ellapsed : %fs\n", interval);
    printf("Average : %d operations/s\n", (int)((double)(nb_add + nb_remove + nb_contain) / interval));

    return 0;
}
