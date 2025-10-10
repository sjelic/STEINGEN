#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "random_graph.h"
#include "random_groups.h"
#include "random_edge_weight.h"
#include "random_vertex_weight.h"

void generate_random_group_steiner_instance(
    uint32_t n, uint64_t m, uint16_t k, const char* e, const char* v, const char* p, char* filename,
    float vertex_a, float vertex_b, const char *vertex_weight_type,
    float edge_a, float edge_b, const char *edge_weight_family, const char *edge_weight_type, float group_penalty_a, float group_penalty_b, const char* group_penalty_type, uint32_t* group_size, const char* group_size_mode, char* group_mode, const char *instance_type, const char* instance_type_code,  uint16_t* seed) {

    

    // Generate a random connected graph
    Edge *edges = (Edge *)malloc(m * sizeof(Edge));
    if (!edges) {
        fprintf(stderr, "Error: Memory allocation failed for edges.\n");
       
        exit(EXIT_FAILURE);
    }
    // uint64_t mmax = (uint64_t)(n * (n - 1) / 2);
    random_connected_graph(n, m, edges, seed);

    // Embed vertices into 2D space for geometry-based edge weights
    float *x = (float *)malloc(n * sizeof(float));
    if (!x) {
        fprintf(stderr, "Error: Memory allocation failed for array x.\n");
        free(edges);
        exit(EXIT_FAILURE);
    }

    float *y = (float *)malloc(n * sizeof(float));
    if (!y) {
        fprintf(stderr, "Error: Memory allocation failed for array y.\n");
        free(x);
        free(edges);
        exit(EXIT_FAILURE);
    }

    embed_vertices(n, 100.0, x, y, seed); // Embedding vertices into [0, 100] space

    
    // Allocate memory for groups
    Group *groups = (Group *)malloc(k * sizeof(Group));
    if (!groups) {
        fprintf(stderr, "Error: Memory allocation failed for groups.\n");
        free(y);
        free(x);
        free(edges);
        exit(EXIT_FAILURE);
    }
    for (unsigned int i = 0; i < k; i++) {
        groups[i].group = (uint32_t *)malloc(n * sizeof(uint32_t));
        groups[i].size = 0;
        if (!groups[i].group) {
            fprintf(stderr, "Error: Memory allocation failed for group %u.\n", i);
            for (unsigned int j = 0; j < i; j++) {
                free(groups[j].group);
            }
            free(groups);
            free(y);
            free(x);
            free(edges);
            exit(EXIT_FAILURE);
        }
    }
    // Generate random groups
    generate_random_groups(n, k, groups, group_size, group_size_mode, group_mode, seed);

    if (strcmp(filename,"") == 0) {
        snprintf(filename, 256, "DWW_%s_%s_n=%u_m=%lu_k=%u_s=%u_vwt=%s_vwl=%.1f_vwu=%.1f_ewt=%s_gs=%u_gsm=%s_gm=%s.stp",
                 instance_type_code,edge_weight_family, n, m, k, (uint32_t)*seed, vertex_weight_type, vertex_a, vertex_b, edge_weight_type, *group_size, group_size_mode, group_mode);
        
    }

    // Open file for writing
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s for writing.\n", filename);
        exit(EXIT_FAILURE);
    }

    // Write the .stp file header
    fprintf(file, "0 string 33D32945  STP Steiner Tree Problem File\n");
    fprintf(file, "SECTION Comment\n");
    fprintf(file, "DWW_%s_%s_n=%u_m=%lu_k=%u_s=%u_vwt=%s_vwl=%.1f_vwu=%.1f_ewt=%s_gs=%u_gsm=%s_gm=%s\n", instance_type_code,edge_weight_family, n, m, k, (uint32_t)*seed, vertex_weight_type, vertex_a, vertex_b, edge_weight_type, *group_size, group_size_mode, group_mode);
    fprintf(file, "Remark '%s'\n",instance_type);
    fprintf(file, "END\n\n");

    // Write the graph section
    fprintf(file, "SECTION Graph\n");
    fprintf(file, "Nodes %u\n", n);
    fprintf(file, "Edges %lu\n", m);

    // Write vertex weights
    if (strcmp(v,"yes") == 0 && strcmp(vertex_weight_type,"int") == 0) {
        for (unsigned int i = 0; i < n; i++) {
            float vertex_weight = generate_random_vertex_weight(vertex_weight_type, vertex_a, vertex_b, seed);
            fprintf(file, "N %.1f\n", vertex_weight); 
        }
    } else if (strcmp(v,"yes") == 0 && strcmp(vertex_weight_type,"float") == 0) {
        for (unsigned int i = 0; i < n; i++) {
            float vertex_weight = generate_random_vertex_weight(vertex_weight_type, vertex_a, vertex_b, seed);
            fprintf(file, "N %.9f\n", vertex_weight); 
        }
    }
    
    if (strcmp(e, "yes") == 0 && strcmp(edge_weight_type,"int") == 0){
        for (unsigned int i = 0; i < m; i++) {
        
            float edge_weight = generate_random_edge_weight(
                x, y, n, edges[i].src, edges[i].dest, edge_a, edge_b, edge_weight_family, edge_weight_type, seed);
            fprintf(file, "E %d %d %.1f\n", edges[i].src, edges[i].dest, edge_weight);
        }

    } else if (strcmp(e, "yes") == 0 && strcmp(edge_weight_type,"float") == 0) {

        for (unsigned int i = 0; i < m; i++) {
        
            float edge_weight = generate_random_edge_weight(
                x, y, n, edges[i].src, edges[i].dest, edge_a, edge_b, edge_weight_family, edge_weight_type, seed);
            fprintf(file, "E %d %d %.9f\n", edges[i].src, edges[i].dest, edge_weight);
        }

    } else if (strcmp(e, "no") == 0) {

        for (unsigned int i = 0; i < m; i++) {

            fprintf(file, "E %d %d\n", edges[i].src, edges[i].dest);
        }
    }
    
    fprintf(file, "END\n\n");

    // Write the terminals section
    fprintf(file, "SECTION Terminals\n");
    fprintf(file, "Terminals %u\n", k);

    // Write group penalties
    if (strcmp(p,"yes") == 0 && strcmp(group_penalty_type,"int") == 0) {
        for (unsigned int i = 0; i < k; i++) {
            float group_penalty = generate_random_vertex_weight(group_penalty_type, group_penalty_a, group_penalty_b, seed);
            fprintf(file, "P %.1f\n", group_penalty); 
        }
    } else if (strcmp(p,"yes") == 0 && strcmp(group_penalty_type,"float") == 0) {
        for (unsigned int i = 0; i < k; i++) {
            float group_penalty = generate_random_vertex_weight(group_penalty_type, group_penalty_a, group_penalty_b, seed);
            fprintf(file, "P %.9f\n", group_penalty); 
        }
    }




    for (unsigned int i = 0; i < k; i++) {
        fprintf(file, "T");
        for (unsigned int j = 0; j < groups[i].size; j++) {
            fprintf(file, " %u", groups[i].group[j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "END\n\n");

    // Write EOF
    fprintf(file, "EOF\n");

    // Clean up
    for (unsigned int i = 0; i < k; i++) {
        free(groups[i].group);
    }
    free(groups);
    free(y);
    free(x);
    free(edges);
    fclose(file);
}
