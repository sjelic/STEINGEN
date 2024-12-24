#include "random_groups.h"
#include "random_graph.h"

// Helper function to shuffle an array
void shuffle(unsigned int *array, unsigned int size) {
    for (unsigned int i = size - 1; i > 0; i--) {
        unsigned int j = rand() % (i + 1);
        unsigned int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}


void generate_random_groups(unsigned int n, unsigned int k, Group **groups_out) {
    if (k > n) {
        fprintf(stderr, "Error: Number of groups (k) cannot exceed the number of vertices (n).\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for array N (vertices) and initialize
    unsigned int *N = (unsigned int *)malloc(n * sizeof(unsigned int));
    if (!N) {
        fprintf(stderr, "Error: Memory allocation failed for array N.\n");
        exit(EXIT_FAILURE);
    }
    for (unsigned int i = 0; i < n; i++) {
        N[i] = i;
    }

    // Allocate memory for groups
    Group *groups = (Group *)malloc(k * sizeof(Group));
    if (!groups) {
        fprintf(stderr, "Error: Memory allocation failed for groups.\n");
        free(N);
        exit(EXIT_FAILURE);
    }
    for (unsigned int i = 0; i < k; i++) {
        groups[i].group = (unsigned int *)malloc(n * sizeof(unsigned int));
        groups[i].size = 0;
        if (!groups[i].group) {
            fprintf(stderr, "Error: Memory allocation failed for group %u.\n", i);
            for (unsigned int j = 0; j < i; j++) {
                free(groups[j].group);
            }
            free(groups);
            free(N);
            exit(EXIT_FAILURE);
        }
    }

    // Generate a random float f in the range (0, 0.5)
    float f;
    do {
        f = (float)rand() / RAND_MAX;
    } while (f <= 0.0 || f >= 0.5);

    unsigned int offset = (unsigned int)(f * n);

    // Shuffle the array N
    shuffle(N, n);

    // Distribute elements to the first part of each group
    for (unsigned int i = 0; i < k; i++) {
        groups[i].group[groups[i].size++] = N[offset + i];
    }

    // Distribute remaining elements to groups
    for (unsigned int i = 0; i < k; i++) {
        unsigned int start = offset + k;
        unsigned int end = offset + k + (n - offset) / k - 1;
        shuffle(N + start, end - start + 1);

        for (unsigned int j = start; j <= end; j++) {
            groups[i].group[groups[i].size++] = N[j];
        }
    }

    // Sort each group
    for (unsigned int i = 0; i < k; i++) {
        qsort(groups[i].group, groups[i].size, sizeof(unsigned int), cmpInt);
    }

    // Clean up
    free(N);
    *groups_out = groups;
}
