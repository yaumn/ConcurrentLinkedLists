# Concurrent linked-list

## Compile

- To compile all the different versions of the linked list (**examples + benchmarks**), type `make` or `make all`
- To compile all the different versions of the linked list (**only the examples**), type `make examples`
- To compile all the different versions of the linked list (**only the benchmarks**), type `make benchmarks`
- To compile a specific version of the linked list (**example + benchmark**), type `make VERSION`.
VERSION can take the following values : one_lock, lock_coupling, optimistic_locking, lazy_locking or lock_free.
Ex: to compile the Lazy locking version, type `make lazy_locking`.
- To compile a specific version of the linked list (**only the example**), type `make VERSION_example`.
Ex: `make one_lock_example`
- To compile a specific version of the linked list (**only the benchmark**), type `make VERSION_benchmark`.
Ex: `make lock_free_benchmark`

## Execute an example

To execute a compiled example, go to the bin directory and execute the example you want.
Ex: `cd bin && ./lock_free_example`

## Execute a benchmark

The benchmark takes some parameters (all of them have default values):

- t: the number of threads to create (default: 4)
- a: the number of add calls (default: 500000)
- r: the number of remove calls (default: 500000)
- c: the number of contain calls (default: 1000000)
- s: the seed to use to generate random number (default: current time)
- i: the initial set size (filled with random values) (default: 300)
- m: the maximum value that can be added to the set (**Do not set a maximum value less than the size of the set, otherwise the set cannot be filled**) (default: 4000)

To execute a compiled benchmark, go to the bin directory and execute the benchmark you want.
Ex: `cd bin && ./lock_free_benchmark -t 3 -a 3000 -r 2000 -c 10000 -s 1274567808 -i 200 -m 4000`
