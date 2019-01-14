SRC=src
BIN=bin
GCC_FLAGS=-Wall -Wextra -std=c11 -D_REENTRANT


all: one_lock lock_coupling optimistic_locking lazy_locking lock_free


examples: one_lock_example lock_coupling_example optimistic_locking_example lazy_locking_example lock_free_example


benchmarks: one_lock_benchmark lock_coupling_benchmark optimistic_locking_benchmark lazy_locking_benchmark lock_free_benchmark


one_lock: one_lock_example one_lock_benchmark

one_lock_example: $(SRC)/one_lock/set_one_lock.c $(SRC)/main.c
	gcc $(GCC_FLAGS) -DONE_LOCK -I $(SRC)/one_lock -lpthread $^ -o $(BIN)/$@

one_lock_benchmark: $(SRC)/one_lock/set_one_lock.c $(SRC)/benchmark.c
	gcc $(GCC_FLAGS) -DONE_LOCK -I $(SRC)/one_lock -lpthread $^ -o $(BIN)/$@


lock_coupling: lock_coupling_example lock_coupling_benchmark

lock_coupling_example: $(SRC)/lock_coupling/set_lock_coupling.c $(SRC)/main.c
	gcc $(GCC_FLAGS) -DLOCK_COUPLING -I $(SRC)/lock_coupling -lpthread $^ -o $(BIN)/$@

lock_coupling_benchmark: $(SRC)/lock_coupling/set_lock_coupling.c $(SRC)/benchmark.c
	gcc $(GCC_FLAGS) -DLOCK_COUPLING -I $(SRC)/lock_coupling -lpthread $^ -o $(BIN)/$@


optimistic_locking: optimistic_locking_example optimistic_locking_benchmark

optimistic_locking_example: $(SRC)/optimistic_locking/set_optimistic_locking.c $(SRC)/main.c
	gcc $(GCC_FLAGS) -DOPTIMISTIC_LOCKING -I $(SRC)/optimistic_locking -lpthread $^ -o $(BIN)/$@ 

optimistic_locking_benchmark: $(SRC)/optimistic_locking/set_optimistic_locking.c $(SRC)/benchmark.c
	gcc $(GCC_FLAGS) -DOPTIMISTIC_LOCKING -I $(SRC)/optimistic_locking -lpthread $^ -o $(BIN)/$@ 


lazy_locking: lazy_locking_example lazy_locking_benchmark 

lazy_locking_example: $(SRC)/lazy_locking/set_lazy_locking.c $(SRC)/main.c
	gcc $(GCC_FLAGS) -DLAZY_LOCKING -I $(SRC)/lazy_locking -lpthread $^ -o $(BIN)/$@

lazy_locking_benchmark: $(SRC)/lazy_locking/set_lazy_locking.c $(SRC)/benchmark.c
	gcc $(GCC_FLAGS) -DLAZY_LOCKING -I $(SRC)/lazy_locking -lpthread $^ -o $(BIN)/$@ 


lock_free: lock_free_example lock_free_benchmark

lock_free_example: $(SRC)/lock_free/set_lock_free.c $(SRC)/main.c
	gcc $(GCC_FLAGS) -DLOCK_FREE -I $(SRC)/lock_free -lpthread $^ -o $(BIN)/$@

lock_free_benchmark: $(SRC)/lock_free/set_lock_free.c $(SRC)/benchmark.c
	gcc $(GCC_FLAGS) -DLOCK_FREE -I $(SRC)/lock_free -lpthread $^ -o $(BIN)/$@


clean:
	rm -f $(BIN)/*
