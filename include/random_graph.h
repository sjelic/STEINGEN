#ifndef RANDOM_GRAPH_H
#define RANDOM_GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "rand_large.h"




// Edge structure
typedef struct Edge {
    uint32_t src;
    uint32_t dest;
} Edge;

// Function prototypes
int cmpInt(const void *a, const void *b);
uint64_t permuted(uint64_t pos, uint64_t a, uint64_t c, uint64_t n);
int8_t binarySearch(uint64_t *arr, uint64_t size, uint64_t key);
uint64_t uv2index(uint32_t u, uint32_t v, uint32_t n);
uint64_t min(uint64_t u, uint64_t v);
uint64_t max(uint64_t u, uint64_t v);
int8_t index2uv(uint64_t index, uint32_t n, uint32_t *u, uint32_t *v);
void random_connected_graph(
    uint32_t n, uint64_t m, Edge *edges, uint16_t* seed);

#endif // RANDOM_GRAPH_H
