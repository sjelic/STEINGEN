#ifndef GEOMETRY_WEIGHTS_H
#define GEOMETRY_WEIGHTS_H

#include <stdlib.h>
#include <stdint.h>

// Function to embed vertices into 2D space
void embed_vertices(int N, float B, float *x, float *y, uint16_t* seed);

// Function to generate random edge weight
float generate_random_edge_weight(float *x, float *y, unsigned int N, unsigned int s, unsigned int t, 
                                   float a, float b, const char *family, const char *type, uint16_t* seed);

#endif // GEOMETRY_WEIGHTS_H
