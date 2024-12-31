#ifndef RANDOM_GROUP_STEINER_H
#define RANDOM_GROUP_STEINER_H

#include "random_groups.h"
#include "random_graph.h"

// Function to generate a random Group Steiner instance
void generate_random_group_steiner_instance(
    uint32_t n, uint32_t m, uint16_t k, const char *filename,
    float vertex_a, float vertex_b, const char *vertex_weight_type,
    float edge_a, float edge_b, const char *edge_weight_family, const char *edge_weight_type, const char *instance_type, uint16_t* seed);

#endif // RANDOM_GROUP_STEINER_H
