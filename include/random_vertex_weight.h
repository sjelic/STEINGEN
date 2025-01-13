#ifndef RANDOM_VERTEX_WEIGHT_H
#define RANDOM_VERTEX_WEIGHT_H

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

// Function to generate a random vertex weight
float generate_random_vertex_weight(const char *weight_type, float a, float b, uint16_t* seed);

#endif // RANDOM_VERTEX_WEIGHT_H
