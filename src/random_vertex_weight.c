#include "random_vertex_weight.h"
#include <stdio.h>

float generate_random_vertex_weight(const char *weight_type, float a, float b, uint16_t* seed) {
    if (a > b) {
        fprintf(stderr, "Error: Lower bound 'a' cannot be greater than upper bound 'b'.\n");
        exit(EXIT_FAILURE);
    }
    float random_value = 1 + a + ((float)rand_r((unsigned int* )seed) / RAND_MAX) * (b - a);

    // Check the weight type and cast appropriately
    if (strcmp(weight_type, "integer") == 0) {
        return (float)((int)random_value);
    } else if (strcmp(weight_type, "float") == 0) {
        return random_value;
    } else {
        fprintf(stderr, "Error: Invalid weight type '%s'. Use 'integer' or 'float'.\n", weight_type);
        exit(EXIT_FAILURE);
    }
}
