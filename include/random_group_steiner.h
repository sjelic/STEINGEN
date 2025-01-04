#ifndef RANDOM_GROUP_STEINER_H
#define RANDOM_GROUP_STEINER_H

#include "random_groups.h"
#include "random_graph.h"

// Function to generate a random Group Steiner instance
void generate_random_group_steiner_instance(
    uint32_t n, uint64_t m, uint16_t k, const char* e, const char* v, const char* p, char* filename,
    float vertex_a, float vertex_b, const char *vertex_weight_type,
    float edge_a, float edge_b, const char *edge_weight_family, const char *edge_weight_type, float group_penalty_a, float group_penalty_b, const char* group_penalty_type, uint32_t* group_size, const char* group_size_mode, char* group_mode, const char *instance_type, const char* instance_type_code, uint16_t* seed);

#endif // RANDOM_GROUP_STEINER_H
