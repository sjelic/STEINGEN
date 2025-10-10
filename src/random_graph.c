#include "random_graph.h"

int cmpInt(const void *a, const void *b)
{
    int64_t int_a = *(int64_t *)a;
    int64_t int_b = *(int64_t *)b;

    // Return the difference (int_a - int_b)
    return (int)((int_a > int_b) - (int_a < int_b)); // Returns -1, 0, or 1
}

inline uint64_t min(uint64_t u, uint64_t v)
{
    return u < v ? u : v;
}

inline uint64_t max(uint64_t u, uint64_t v)
{
    return u < v ? v : u;
}

inline uint64_t uv2index(uint32_t u, uint32_t v, uint32_t n)
{
    if (u < n && v < n && u < v)
    {
        uint64_t uu = (uint64_t)(u);
        uint64_t vv = (uint64_t)(v);
        uint64_t nn = (uint64_t)(n);
        uint64_t mmax = (nn * (nn - 1)) / 2;
        uint64_t index = (uu * (2 * nn - uu - 1)) / 2 + (vv - uu - 1);
        if (index < mmax)
        {
            return (uint64_t)index;
        }
        else
        {
            fprintf(stderr, "Error: Formula should be checked for index calculation.\n");
            return (uint64_t)(-1);
        }
    }
    fprintf(stderr, "Error: Invalid u or v values. u = %u, v = %u, n=%u \n", u, v, n);
    return (uint64_t)(-1);
}

inline int8_t index2uv(uint64_t index, uint32_t n, uint32_t *u, uint32_t *v)
{
    int64_t nn = (int64_t)n;
    int64_t indexx = (int64_t)index;
    int64_t dd = (2 * nn - 1) * (2 * nn - 1) - 8 * indexx;
    // double uur = ((double)(2 * n - 1) - sqrt(dd)) / 2;
    int64_t uu = ((double)(2 * nn - 1) - sqrt(dd)) / 2;
    int64_t vv = uu + index + 1 - uu * (2 * nn - uu - 1) / 2;
    if (indexx < (nn * (nn - 1)) / 2)
    {
        if (uu >= 0 && uu < n && vv >= uu + 1 && vv < n)
        {
            *u = (uint32_t)uu;
            *v = (uint32_t)vv;
            return 0;
        }
        else
        {
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

inline uint64_t permuted(uint64_t pos, uint64_t a, uint64_t c, uint64_t n)
{
    return (((uint64_t)a) * ((uint64_t)pos) + (uint64_t)c) % ((uint64_t)n);
}

// Custom binary search function
inline int8_t binarySearch(uint64_t *arr, uint64_t size, uint64_t key)
{
    int64_t low = 0, high = size - 1;
    while (low <= high)
    {
        int64_t mid = low + (high - low) / 2;
        if (arr[mid] == key)
            return 1; // Key found
        if (arr[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return 0; // Key not found
}

/**
 * @brief Generates a random connected graph with the specified number of nodes and edges.
 *
 * This function creates a random connected graph with `n` nodes and `m` edges.
 * The edges of the graph are stored in the provided `edges` array. The graph
 * generation process uses the given `seed` for randomization, ensuring
 * reproducibility of the generated graph.
 *
 * @param n The number of nodes in the graph.
 * @param m The number of edges in the graph.
 * @param edges Pointer to an array where the generated edges will be stored.
 *              The array should be pre-allocated with a size of at least `m`.
 * @param seed Pointer to a 16-bit seed value used for random number generation.
 *             The seed is updated during the graph generation process.
 */
void random_connected_graph(uint32_t n, uint64_t m, Edge *edges, uint16_t *seed)
{
    if (seed == NULL)
    {
        fprintf(stderr, "Error: Seed parameter is NULL.\n");
        exit(EXIT_FAILURE);
    }
    if (m > n * (n - 1) / 2)
    {
        fprintf(stderr, "Error: Number of edges exceeds the maximum possible for %u vertices.\n", n);
        exit(EXIT_FAILURE);
    }

    uint64_t PRIME_PERM = 5000000029;
    uint64_t mmax = (uint64_t)n * ((uint64_t)n - 1) / 2; // Maximum possible number of edges
    uint64_t *T = malloc(m * sizeof(uint64_t));          // Array for selected edge indices
    if (!T)
    {
        fprintf(stderr, "Error: Memory allocation failed for array T.\n");
        exit(EXIT_FAILURE);
    }

    // Select coprime multiplier `a`

    // uint32_t c = *seed % n; // Random offset
    uint32_t c = 0;
    // Step 1: Build a spanning tree
    for (uint64_t v = 1; v < n; v++)
    {
        uint64_t u = rand_r_large(seed) % v;
        uint64_t permutedV = permuted(v, PRIME_PERM, c, n);
        uint64_t permutedU = permuted(u, PRIME_PERM, c, n);

        if (min(permutedU, permutedV) == max(permutedU, permutedV))
        {
            free(T);
            fprintf(stderr, "u and v are not mapped to different indices: u=%lu, v=%lu, min = %lu\n", u, v, min(permutedU, permutedV));
            exit(EXIT_FAILURE);
        }

        uint64_t index = uv2index(min(permutedU, permutedV), max(permutedU, permutedV), n);
        if (index == (uint64_t)(-1))
        {
            free(T);
            exit(EXIT_FAILURE);
        }
        T[v - 1] = index; // Add spanning tree edge index
    }

    // Sort the first n-1 elements for binary search
    qsort(T, n - 1, sizeof(uint64_t), cmpInt);
    uint64_t previous_index = mmax; // Initialize with an invalid index
    for (uint64_t i = 0; i < n - 1; i++)
    {
        uint32_t u, v;
        if (index2uv(T[i], n, &u, &v) == -1)
        {
            free(T);
            exit(EXIT_FAILURE);
        }

        // Check for duplicate edges
        if (T[i] == previous_index)
        {
            fprintf(stderr, "ERROR: Duplicate edge (%u, %u) detected, T[i] = %lu, previous_index = %lu.\n", u, v, T[i], previous_index);
            free(T);
            exit(EXIT_FAILURE);
        }
        previous_index = T[i];

        // edges[i].src = u;
        // edges[i].dest = v;
    }
    // Step 2: Add additional edges
    uint64_t additional_edges_added = 0;
    // uint64_t current = 0;

    for (uint64_t i = 0; i < mmax && additional_edges_added < m - n + 1; i++)
    {
        uint64_t permutedIndex = permuted(i, PRIME_PERM, c, mmax);

        // Check if the edge index is already in the spanning tree
        if (binarySearch(T, n - 1, permutedIndex))
        {
            continue; // Edge already exists in the tree
        }

        // Add the edge index to T after n-1 elements
        T[n - 1 + additional_edges_added] = permutedIndex;
        additional_edges_added++;
    }

    // Verify enough edges were added
    if (additional_edges_added != m - n + 1)
    {
        fprintf(stderr, "Error: Could not generate the required number of edges.\n");
        free(T);
        exit(EXIT_FAILURE);
    }

    // c = *seed % mmax;
    c = 0;
    // Step 3: Sort the entire T array
    qsort(T, m, sizeof(uint64_t), cmpInt);

    // Step 4: Convert edge indices to actual edges
    previous_index = mmax; // Initialize with an invalid index
    for (uint64_t i = 0; i < m; i++)
    {
        uint32_t u, v;
        if (index2uv(T[i], n, &u, &v) == -1)
        {
            free(T);
            exit(EXIT_FAILURE);
        }

        // Check for duplicate edges
        if (T[i] == previous_index)
        {
            fprintf(stderr, "Error: Duplicate edge (%u, %u) detected, T[i] = %lu, previous_index = %lu.\n", u, v, T[i], previous_index);
            free(T);
            exit(EXIT_FAILURE);
        }
        previous_index = T[i];

        edges[i].src = u;
        edges[i].dest = v;
    }

    // Free dynamically allocated memory
    free(T);
}
