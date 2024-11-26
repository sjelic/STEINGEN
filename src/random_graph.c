#include "random_graph.h"

int cmpInt(const void *a, const void *b) {
    return (*(unsigned int *)a - *(unsigned int *)b);
}


inline unsigned int min(unsigned int u, unsigned int v){
    return u < v  ? u : v;
}

inline unsigned int max(unsigned int u, unsigned int v){
    return u < v ? v : u;
}

unsigned int uv2index(unsigned int u, unsigned int v, unsigned int n) {
    if (u < n && v < n && u < v) {
        unsigned int index = (u * (2*n - u - 1) / 2) + (v - u - 1);
        if (index < (n * (n - 1)) / 2) {
            return index;
        } else {
            fprintf(stderr, "Error: Formula should be checked for index calculation.\n");
            return (unsigned int)-1;
        }
    }
    fprintf(stderr, "Error: Invalid u or v values.\n");
    return (unsigned int)(-1);
}

int index2uv(unsigned int index, unsigned int n, int *u, int *v) {
    if (index < (n * (n - 1)) / 2) {
        long long nn = (long long)n;
        long long indexx = (long long)index;
        long long dd = (2 * nn - 1) * (2 * nn - 1) - 8 * indexx;
        double uur = ((double)(2 * n - 1) - sqrt(dd)) / 2;
        long long uu = ((double)(2 * n - 1) - sqrt(dd)) / 2;
        long long vv = uu + index + 1 - uu * (2*n - uu - 1) / 2;
        if (uu >= 0 && uu < n && vv >= uu + 1 && vv < n) {
            *u = (unsigned int)uu;
            *v = (unsigned int)vv;
            return 0;
        } else {
            printf("n: %lld\n", nn);
            printf("index: %lld\n", indexx);
            printf("D: %lld\n", dd);
            printf("u: %.10f\n", uur);
            fprintf(stderr, "Error: Invalid calculated u or v values. Index: %d, u = %lld, v=%lld\n", index, uu, vv);
            return -1;
        }
    }
    fprintf(stderr, "Error: Invalid edge index. Must be less than n(n-1)/2.\n");
    return -1;
}

void random_spanning_tree(
    unsigned int n, unsigned int m, unsigned int seed, Edge *edges) {

    if (m > n * (n - 1) / 2) {
        fprintf(stderr, "Error: Number of edges exceeds the maximum possible for %u vertices.\n", n);
        exit(EXIT_FAILURE);
    }

    unsigned int mmax = n * (n - 1) / 2; // Maximum possible number of edges
    unsigned int *N = malloc(n * sizeof(unsigned int));    // Vertices array
    unsigned int *M = malloc(mmax * sizeof(unsigned int)); // All edge indices
    unsigned int *T = malloc(m * sizeof(unsigned int));    // Selected edges for the graph
    if (!N || !M || !T) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        free(N); free(M); free(T);
        exit(EXIT_FAILURE);
    }

    // Initialize vertex and edge index arrays
    for (unsigned int i = 0; i < n; i++) N[i] = i;
    for (unsigned int i = 0; i < mmax; i++) M[i] = i;

    // Seed random number generator and shuffle vertices
    srand(seed);
    for (unsigned int i = n - 1; i > 0; i--) {
        unsigned int j = rand() % (i + 1);
        unsigned int temp = N[i];
        N[i] = N[j];
        N[j] = temp;
    }

    // Build spanning tree by randomly selecting edges
    for (unsigned int v = 1; v < n; v++) {
        unsigned int u = rand() % v;
        unsigned int index = uv2index(min(N[u],N[v]), max(N[u],N[v]), n);
        if (index == (unsigned int)-1) {
            free(N); free(M); free(T);
            exit(EXIT_FAILURE);
        }
        T[v - 1] = index; // Add edge index to T
    }

    // Sort the initial edges of the spanning tree
    qsort(T, n - 1, sizeof(unsigned int), cmpInt);

    // put all selected edges back to the array (last n-1 elements of M)
    for (int i = n-2; i >= 0; i--) {
        unsigned int temp = M[mmax - 1 - n + 2 + i];
        M[mmax - 1 - n + 2 + i] = M[T[i]];
        M[T[i]] = temp;
    }


    // Randomly shuffle remaining edges in M[0,...,mmax - n]
    for (unsigned int i = mmax - n; i > 0; i--) {
        unsigned int j = rand() % (i + 1);
        unsigned int temp = M[i];
        M[i] = M[j];
        M[j] = temp;
    }

    // Select the first m - n + 1 additional edges
    for (unsigned int i = 0; i < m - n + 1; i++) {
        T[n - 1 + i] = M[i];
    }

    // Sort the entire T array
    qsort(T, m, sizeof(unsigned int), cmpInt);

    // Convert indices to edges and create the edge array
    
    unsigned int check = m;
    for (unsigned int i = 0; i < m; i++) {
        int u, v;
        
        if (index2uv(T[i], n, &u, &v) == -1) {
            free(N); free(M); free(T); free(edges);
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
    free(N);
    free(M);
    free(T);
}
