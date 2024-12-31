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
uint64_t uv2index(uint32_t u, uint32_t v, uint32_t n);
uint32_t min(uint32_t u, uint32_t v);
uint32_t max(uint32_t u, uint32_t v);
int8_t index2uv(uint64_t index, uint32_t n, uint32_t *u, uint32_t *v);
void random_connected_graph(
    uint32_t n, uint64_t m, Edge *edges, uint16_t* seed);

#endif // RANDOM_GRAPH_H
