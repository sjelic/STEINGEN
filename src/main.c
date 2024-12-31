#include <stdio.h>
#include "random_group_steiner.h"

int main() {
    uint32_t n = 90000;       // Number of vertices
    uint64_t m = 1000000;       // Number of edges
    uint16_t k = 2000;       // Number of groups
    uint16_t seed = 12345;



    const char *filename = "random_instance.stp";
    float vertex_a = 1.0, vertex_b = 10.0;
    const char *vertex_weight_type = "float";
    float edge_a = 1.0, edge_b = 10.0;
    const char *edge_weight_family = "random";
    const char *edge_weight_type = "float";
    const char *instance_type = "Node Weighted Group Steiner Tree Instance";

    generate_random_group_steiner_instance(
        n, m, k, filename,
        vertex_a, vertex_b, vertex_weight_type,
        edge_a, edge_b, edge_weight_family, edge_weight_type, instance_type, &seed);

    printf("Random Group Steiner instance written to %s\n", filename);
    return 0;
}
