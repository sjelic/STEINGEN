#include "random_groups.h"
#include "random_graph.h"
#include <string.h>



int cmpGroups(const void *a, const void *b) {
    uint32_t int_a = *(uint32_t *)a;
    uint32_t int_b = *(uint32_t *)b;

    // Return the difference (int_a - int_b)
    return (int) ((int_a > int_b) - (int_a < int_b)); // Returns -1, 0, or 1
}


// Helper function to shuffle an array
void shuffle(uint32_t *array, uint32_t size, uint16_t* seed) {
    for (uint32_t i = size - 1; i > 0; i--) {
        uint32_t j = rand_r((unsigned int* )seed) % (i + 1);
        uint32_t temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}


void generate_random_groups(uint32_t n, uint16_t k, Group *groups, uint32_t* group_size, const char* group_size_mode, char* group_mode, uint16_t* seed) {
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



    uint32_t total;

    if(strcmp(group_size_mode, "auto") == 0){
        total = (n - offset) / k - 1;
        *group_size = total + 1;

    }else if (strcmp(group_size_mode, "manual") == 0){

        if (*group_size  >  n - offset - k ) {
            fprintf(stderr, "Warning: Group size is shrinked to %d.\n", n - offset - k);
            *group_size = n - offset - k;
        }
        total = *group_size - 1;


        if(strcmp(group_mode, "disjoint") == 0){
            if ( *group_size > (n - offset) / k - 1){
                snprintf(group_mode, 256, "overlap");
                fprintf(stderr, "Warning: Groups cannot be disjoint, because group size is too large: %d. It has to be at most %d. Switched to overlapping instance.\n", *group_size, (n - offset) / k - 1);
                
            }
        }


    }else{
        fprintf(stderr, "Error: Group size mode %s is not supported.\n", group_size_mode);
        free(I);
        free(N);
        exit(EXIT_FAILURE);
    }


    if (strcmp(group_mode, "overlap") == 0){
        for (uint32_t i = 0; i < k; i++) {
            
            shuffle(I,npool,seed);
            for (uint32_t j = 0; j < total; j++) {            
                groups[i].group[groups[i].size++] = N[offset + k + I[j]];
            }
        }
    }
    else if (strcmp(group_mode, "disjoint") == 0){
        // shuffle(I,npool,seed);
        for (uint32_t i = 0; i < k; i++) {
            for (uint32_t j = 0; j < total; j++) {            
                groups[i].group[groups[i].size++] = N[offset + k + i*total + I[j]];
            }
        }
    }
    else{
        fprintf(stderr, "Error: Parameter 'group_mode' can be either 'overlap' or 'disjoint', but %s provided.\n", group_mode);
        free(I);
        free(N);
        exit(EXIT_FAILURE);

    }

    //Sort each group
    for (uint32_t i = 0; i < k; i++) {
        qsort(groups[i].group, groups[i].size, sizeof(uint32_t), cmpGroups);
    }

    // Clean up
    free(I);
    free(N);
}
