/**
 * CS61064 - High Perfomance Parallel Programming
 * OpenMP/MPI - Assignment 2
 * Twin Prime Numbers
 * 
 * Author: Utkarsh Patel (18EC35034)
 *
 * Parallel code for twin prime generation using OpenMPI
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <mpi.h>

#include "logging.h"
#include "utils.h"


int main(int argc, char **argv) {
    if (argc != 3) {
        LOG_FATAL("Error: Run the executable as:\n$ %s m K\n", argv[0]);
    }

    int    rank;         /* The rank for current process in MPI */
    int    numranks;     /* Number of processes spawned in MPI */
    int    m;            /* The `m` in `mk + 1` */
    int    k;            /* The `k` in `mk + 1` */
    int    K;            /* Maximum value of `k` */
    int    N;            /* `N = mK + 1` */
    int    S;            /* `S = sqrt(N)` */
    int    low;          /* The first number to consider for a given process */
    int    high;         /* The last number to consider for a given process */ 
    char   *base;        /* `base` stores whether the numbers `0`, `1`, ..., `S` are primes or not */
    char   *block;       /* `block` stores whether the numbers `low`, `low + 1`, ..., `high` are primes or not */
    int    block_size;   /* `block_size = high - low + 1` */
    int    rem;          /* `rem = (N - (S + 1)) % numranks `*/
    int    local_count;  /* Number of local twin prime pairs found for each process */
    int    global_count; /* Total number of twin prime pairs */
    double start_time;   /* Time at which the computation starts */
    double elapsed_time; /* Time elapsed when the computation ends */ 
    int    i, j, k1, k2; /* Used for iteration */

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);

    m = atoi(argv[1]);
    K = atoi(argv[2]);
    N = m * K + 2;
    S = (int) sqrt(N);

    /**
     * Every process will compute whether the numbers `0`, `1`, ..., `S` are prime or not.
     * Then, they will use this computation to find other prime numbers in their local block.
     * Hence, we need to divide `N - (S + 1)` numbers between `numranks` processes.
     */
    block_size = (N - (S + 1)) / numranks;
    rem        = (N - (S + 1)) % numranks;
    low        = (S + 1) + rank * block_size;
    high       = low + block_size - 1;

    /**
     * As it is not necessary that `N - (S + 1)` will be a multiple of `numranks`. Hence, the
     * extra numbers will be processed by the process with the last rank.
     */
    if (rank == (numranks - 1)) high += rem;

    base =  (char *) malloc((S + 1) * sizeof(char));
    if (base == NULL) {
        LOG_FATAL("Error: cannot allocate 'base' array for rank %d\n", rank);
    }

    block = (char *) malloc((high - low + 1) * sizeof(char));
    if (block == NULL) {
        LOG_FATAL("Error: cannot allocate 'block' array for rank %d\n", rank);
    }

    MPI_Barrier(MPI_COMM_WORLD);
	start_time = MPI_Wtime();

    /* Sieve of Erathosthenes used to fill `base` and `block` arrays */

    for (i = 0; i <= S; i++) base[i] = 1;
    for (i = low; i <= high; i++) block[i - low] = 1;

    base[0] = base[1] = 0;

    for (i = 2; i <= S; i++) {
        if (base[i] == 0) continue;
        for (j = i * i; j <= S; j += i) base[j] = 0;
        for (j = begin_multiple(i, low); j < end_multiple(i, high); j += i) {
            assert(j >= low);
            assert(j <= high);
            block[j - low] = 0;
        }
    }

    /* Find twin primes in process's local `block` array */

    local_count = 0;

    i  = begin_A(m, low);
    j  = begin_B(m, low);
    k1 = MIN(i, j);
    i  = end_A(m, high);
    j  = end_B(m, high);
    k2 = MIN(i, j);

    for (k = k1; k < k2; k++) {
        i = m * k - 1;
        j = m * k + 1;
        if (i < low)  i = is_prime(i);
        else i = block[i - low];
        j = block[j - low];
        local_count += (i & j);
    }

    /* As all process have same `base` array, let process 0 find twins primes in it */
    if (rank == 0) {
        k1 = 1;
        i  = end_A(m, S);
        j  = end_B(m, S);
        k2 = MIN(i, j);

        for (k = k1; k < k2; k++) {
            i = m * k - 1;
            j = m * k + 1;
            i = base[i];
            j = base[j];
            local_count += (i & j);
        }
    }

    /* Compute total number of twin primes */
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    elapsed_time = MPI_Wtime() - start_time;

    if (rank == 0) {
        LOG_INFO("%d twin prime pairs found in %.3lfms.\n", global_count, elapsed_time * 1000);
    }

    free(base);
    free(block);

    MPI_Finalize();

    return 0;
}