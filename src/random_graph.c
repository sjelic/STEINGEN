#include "random_graph.h"


int cmpInt(const void *a, const void *b) {
    int64_t int_a = *(int64_t *)a;
    int64_t int_b = *(int64_t *)b;

    // Return the difference (int_a - int_b)
    return (int) ((int_a > int_b) - (int_a < int_b)); // Returns -1, 0, or 1
}


inline uint32_t min(uint32_t u, uint32_t v){
    return u < v  ? u : v;
}

inline uint32_t max(uint32_t u, uint32_t v){
    return u < v ? v : u;
}

uint64_t uv2index(uint32_t u, uint32_t v, uint32_t n) {
    if (u < n && v < n && u < v) {
        uint64_t uu = (uint64_t)(u);
        uint64_t vv = (uint64_t)(v);
        uint64_t nn = (uint64_t)(n);
        uint64_t mmax = (nn * (nn - 1)) / 2;
        uint64_t index = (uu * (2*nn - uu - 1))/2 + (vv - uu - 1);
        if (index < mmax) {
            return (uint64_t)index;
        } else {
            fprintf(stderr, "Error: Formula should be checked for index calculation.\n");
            return (uint64_t)(-1);
        }
    }
    fprintf(stderr, "Error: Invalid u or v values.\n");
    return (uint64_t)(-1);
}

int8_t index2uv(uint64_t index, uint32_t n, uint32_t *u, uint32_t *v) {
    if (index < (n * (n - 1)) / 2) {
        int64_t nn = (int64_t)n;
        int64_t indexx = (int64_t)index;
        int64_t dd = (2 * nn - 1) * (2 * nn - 1) - 8 * indexx;
        // double uur = ((double)(2 * n - 1) - sqrt(dd)) / 2;
        int64_t uu = ((double)(2 * n - 1) - sqrt(dd)) / 2;
        int64_t vv = uu + index + 1 - uu * (2*n - uu - 1) / 2;
        if (uu >= 0 && uu < n && vv >= uu + 1 && vv < n) {
            *u = (uint32_t)uu;
            *v = (uint32_t)vv;
            return 0;

        } else {
            // printf("n: %lu\n", nn);
            // printf("index: %lu\n", indexx);
            // printf("D: %d\n", dd);
            // printf("u: %.10f\n", uur);
            // fprintf(stderr, "Error: Invalid calculated u or v values. Index: %d, u = %d, v=%d\n", index, uu, vv);
            return -1;
        }
    }
    fprintf(stderr, "Error: Invalid edge index. Must be less than n(n-1)/2.\n");
    return -1;
}

void random_connected_graph(
    uint32_t n, uint64_t m, Edge *edges, uint16_t* seed) {

    if (m > n * (n - 1) / 2) {
        fprintf(stderr, "Error: Number of edges exceeds the maximum possible for %u vertices.\n", n);
        exit(EXIT_FAILURE);
    }

    uint64_t mmax = (uint64_t)n * ((uint64_t)n - 1) / 2; // Maximum possible number of edges
    uint32_t *N = malloc(n * sizeof(uint32_t));    // Vertices array
    uint64_t *M = malloc(mmax * sizeof(uint64_t)); // All edge indices
    uint64_t *T = malloc(m * sizeof(uint64_t));    // Selected edges for the graph
    if (!N || !M || !T) {
        fprintf(stderr, "Error: Memory allocation of arrays N, M or T failed.\n");
       free(T);
            free(M);
            free(N);
            exit(EXIT_FAILURE);
    }

    // Initialize vertex and edge index arrays
    for (uint32_t i = 0; i < n; i++) N[i] = i;
    for (uint64_t i = 0; i < mmax; i++) M[i] = i;

    // Seed random number generator and shuffle vertices
    for (uint32_t i = n - 1; i > 0; i--) {
        uint32_t j = rand_r_large(seed) % (i + 1);
        uint32_t temp = N[i];
        N[i] = N[j];
        N[j] = temp;
    }

    // Build spanning tree by randomly selecting edges
    for (uint32_t v = 1; v < n; v++) {
        uint32_t u = rand_r_large(seed) % v;
        uint64_t index = uv2index(min(N[u],N[v]), max(N[u],N[v]), n);
        if (index == (uint64_t)(-1)) {
            free(T);
            free(M);
            free(N);
            exit(EXIT_FAILURE);
        }
        if (index >= mmax){
            fprintf(stderr, "u = %u; v = %u", u, v);
            fprintf(stderr, "index = %lu; mmax = %lu", index, mmax);
        }
        T[v - 1] = index; // Add edge index to T
    }

    // Sort the initial edges of the spanning tree
    qsort(T, n - 1, sizeof(uint64_t), cmpInt);

    // put all selected edges back to the array (last n-1 elements of M)
    for (int64_t i = n-2; i >= 0; i--) {
        if (T[i] > mmax){
                fprintf(stderr, "T[i] = %lu; mmax = %lu", T[i], mmax);

        }
        uint32_t temp = M[mmax - 1 - n + 2 + i];
        M[mmax - 1 - n + 2 + i] = M[T[i]];
        M[T[i]] = temp;
    }


    // Randomly shuffle remaining edges in M[0,...,mmax - n]
    for (int64_t i = mmax - n; i > 0; i--) {
        uint32_t j = rand_r_large(seed) % (i + 1);
        uint32_t temp = M[i];
        M[i] = M[j];
        M[j] = temp;
    }

    // Select the first m - n + 1 additional edges
    for (uint64_t i = 0; i < m - n + 1; i++) {
        T[n - 1 + i] = M[i];
    }

    // Sort the entire T array
    qsort(T, m, sizeof(uint64_t), cmpInt);

    // Convert indices to edges and create the edge array
    
    uint64_t check = m;
    for (uint64_t i = 0; i < m; i++) {
        uint32_t u, v;
        
        if (index2uv(T[i], n, &u, &v) == -1) {
            free(T);
            free(M);
            free(N);
            exit(EXIT_FAILURE);
        }
        if (T[i] == check){
            fprintf(stderr, "Error: Duplicated edge (%d - %d) found.\n", u, v);
            exit(EXIT_FAILURE);
        }
        check = T[i];

        edges[i].src = u;
        edges[i].dest = v;
    }

    // Output edges

    // Cleanup
    free(T);
    free(M);
    free(N);
}
