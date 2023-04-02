/**
 * CS61064 - High Perfomance Parallel Programming
 * OpenMP/MPI - Assignment 2
 * Twin Prime Numbers
 * 
 * Author: Utkarsh Patel (18EC35034)
 *
 * Helper macros
 */

#ifndef UTILS_H
#define UTILS_H

#define MIN(a, b) ((a) < (b))? (a) : (b)

/**
 * @brief Check whether the given number is prime or not 
 */
inline int is_prime(int x) {
    for (int d = 2; d * d <= x; d++) {
        if (x % d == 0) return 0;
    }
    return 1;
}

/**
 * @brief Get the smallest multiple of `k` no less than `n`
 */
inline int begin_multiple(int k, int n) {
    return n + (k - (n % k)) % k;
}

/**
 * @brief Get the smallest multiple of `k` greater than `n`
 */
inline int end_multiple(int k, int n) {
    return n + k - (n % k);
}

/**
 * @brief Get value of A to begin the loop
 */
inline int begin_A(int m, int n) {
    int A = (n + 1) / m;
    if ((n + 1) % m) A++;
    return A;
}

/**
 * @brief Get value of B to begin the loop
 */
inline int begin_B(int m, int n) {
    int B = (n - 1) / m;
    if ((n - 1) % m) B++;
    return B;
}

/**
 * @brief Get value of A to end the loop
 */
inline int end_A(int m, int n) {
    return (n + 1) / m + 1;
}

/**
 * @brief Get value of B to end the loop
 */
inline int end_B(int m, int n) {
    return (n - 1) / m + 1;
}



#endif // UTILS_H