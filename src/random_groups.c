#include "random_groups.h"
#include "random_graph.h"

// Helper function to shuffle an array
void shuffle(uint32_t *array, uint32_t size, uint16_t* seed) {
    for (uint32_t i = size - 1; i > 0; i--) {
        uint32_t j = rand_r((unsigned int* )seed) % (i + 1);
        uint32_t temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}


void generate_random_groups(uint32_t n, uint16_t k, Group *groups, uint16_t* seed) {
    if (k > n) {
        fprintf(stderr, "Error: Number of groups (k) cannot exceed the number of vertices (n).\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for array N (vertices) and initialize
    uint32_t *N = (uint32_t *)malloc(n * sizeof(uint32_t));
    if (!N) {
        fprintf(stderr, "Error: Memory allocation failed for array N.\n");
        exit(EXIT_FAILURE);
    }
    for (uint32_t i = 0; i < n; i++) {
        N[i] = i;
    }
    

    // Generate a random float f in the range (0, 0.5)
    float f;
    f = (float) ( rand_r((unsigned int* )seed) / 2.0 ) / RAND_MAX;


    uint32_t offset = (uint32_t)(f * n);

    // Shuffle the array N
    shuffle(N, n, seed);
    if (offset + k > n) { 
        fprintf(stderr, "Error: Number of Steiner vertices (%d) plus k (%d) is greather than n (%d). \n", offset, k, n);
        free(N);
        exit(EXIT_FAILURE);
    }


    uint32_t npool = n - offset - k;
    uint32_t *I = (uint32_t *)malloc( npool* sizeof(uint32_t));
    if (!I) {
        fprintf(stderr, "Error: Memory allocation failed for array I.\n");
        free(N);
        exit(EXIT_FAILURE);
    }
    for (uint32_t i = 0; i < npool; i++) {
        I[i] = i;
    }

    // Distribute elements to the first part of each group
    for (uint32_t i = 0; i < k; i++) {
        groups[i].group[groups[i].size++] = N[offset + i];
    }

    uint32_t total = (n - offset) / k - 1;
    for (uint32_t i = 0; i < k; i++) {
        
        shuffle(I,npool,seed);
        for (uint32_t j = 0; j < total; j++) {            
            groups[i].group[groups[i].size++] = N[offset + k + I[j]];
        }
    }

    // Sort each group
    for (uint32_t i = 0; i < k; i++) {
        qsort(groups[i].group, groups[i].size, sizeof(uint32_t), cmpInt);
    }

    // Clean up
    free(I);
    free(N);
}
