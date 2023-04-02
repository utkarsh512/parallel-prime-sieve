/**
 * CS61064 - High Perfomance Parallel Programming
 * OpenMP/MPI - Assignment 2
 * Twin Prime Numbers
 * 
 * Author: Utkarsh Patel (18EC35034)
 *
 * Serial code for twin prime generation
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "logging.h"


int main(int argc, char *argv[]) {
    if (argc != 3) {
        LOG_FATAL("Error: Run the executable as:\n$ %s m K\n", argv[0]);
    }

    int m = atoi(argv[1]);
    int K = atoi(argv[2]);
    int N = m * K + 1;
    
    int twin_prime_count = 0;

    int *is_prime = (int *) malloc(sizeof(int) * (N + 1));

    clock_t timer_begin = clock();

    for (int n = 0; n <= N; n++) {
        is_prime[n] = 1;
    }

    is_prime[0] = is_prime[1] = 0;
    for (int n = 2; n * n <= N; n++) {
        if (is_prime[n]) {
            for (int p = n * n; p <= N; p += n) {
                is_prime[p] = 0;
            }
        }
    }

    for (int k = 1; k <= K; k++) {
        twin_prime_count += (is_prime[m * k - 1] && is_prime[m * k + 1]);
    }

    clock_t timer_end = clock();
    double elapsed_time = (double) (timer_end - timer_begin) / CLOCKS_PER_SEC;
    LOG_INFO("%d twin prime pairs found in %.3lfms.\n", twin_prime_count, elapsed_time * 1000);
    free(is_prime);
    return 0;
}