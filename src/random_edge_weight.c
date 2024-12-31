#include "random_edge_weight.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

// Embed vertices into 2D space with random coordinates between 0 and B
void embed_vertices(int N, float B, float *x, float *y, uint16_t* seed) {
    for (int i = 0; i < N; i++) {
        x[i] = ((float)rand_r((unsigned int* )seed) / RAND_MAX) * B;
        y[i] = ((float)rand_r((unsigned int* )seed) / RAND_MAX) * B;
    }
}

// Generate random edge weight based on the given weight type
float generate_random_edge_weight(float *x, float *y, unsigned int N, unsigned int s, unsigned int t, 
                                  float a, float b, const char *family, const char *type, uint16_t* seed) {
    if (s >= N || t >= N) {
        fprintf(stderr, "Error: Vertex indices s (%u) or t (%u) exceed the number of vertices (%d).\n", s, t, N);
        exit(EXIT_FAILURE);
    }

    float weight = 1.0;

    if (strcmp(family, "euclid") == 0) {
        // Euclidean distance
        float dx = x[s] - x[t];
        float dy = y[s] - y[t];
        weight += sqrt(dx * dx + dy * dy);
    } else if (strcmp(family, "grid") == 0) {
        // Manhattan distance
        weight += fabs(x[s] - x[t]) + fabs(y[s] - y[t]);
    } else if (strcmp(family, "random") == 0) {
        // Uniform random weight
        weight += a + ((float)rand_r((unsigned int* )seed) / RAND_MAX) * (b - a);
    } else {
        fprintf(stderr, "Error: Invalid weight type '%s'. Use 'euclid', 'grid', or 'random'.\n", family);
        exit(EXIT_FAILURE);
    }

    // Cast to integer if the type is "int"
    if (strcmp(type, "int") == 0) {
        weight = (float)((int)weight);
    } else if (strcmp(type, "float") != 0) {
        fprintf(stderr, "Error: Invalid type '%s'. Use 'int' or 'float'.\n", type);
        exit(EXIT_FAILURE);
    }

    return weight;
}
