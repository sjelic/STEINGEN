#ifndef RANDOM_GRAPH_H
#define RANDOM_GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Edge structure
typedef struct Edge {
    int src;
    int dest;
} Edge;

// Function prototypes
int cmpInt(const void *a, const void *b);
unsigned int uv2index(unsigned int u, unsigned int v, unsigned int n);
unsigned int min(unsigned int u, unsigned int v);
unsigned int max(unsigned int u, unsigned int v);
int index2uv(unsigned int index, unsigned int n, int *u, int *v);
void random_spanning_tree(
    unsigned int n, unsigned int m, unsigned int seed, Edge *edges);

#endif // RANDOM_GRAPH_H
