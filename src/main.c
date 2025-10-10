#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include "random_group_steiner.h"
void print_usage(const char *prog_name) {
    printf("Usage: %s [OPTIONS]\n", prog_name);
    printf("\nOptions:\n");
    printf("  -n <vertices>                 Number of vertices (default: 10)\n");
    printf("  -m <edges>                    Number of edges (default: 30)\n");
    printf("  -k <groups>                   Number of groups (default: 4)\n");
    printf("  -s <seed>                     Random seed (default: 12345)\n");
    printf("  -e <edgeweighted>             If 'yes', non-zero edge weights are generated, else if 'no', no weights provided (default: yes)\n");
    printf("  -v <nodeweighted>             If 'yes', non-zero node weights are generated, else if 'no', no weights provided (default: yes)\n");
    printf("  -p <grouppenalties>           If 'yes', group penalites are generated, else if 'no', penalites are not generated (default: no)\n");
    printf("  -f <filename>                 Output filename (default: auto-generated)\n");
    printf("  --vertex-a <float>            Lower bound for vertex weights (default: 1.0)\n");
    printf("  --vertex-b <float>            Upper bound for vertex weights (default: 10.0)\n");
    printf("  --vertex-type <type>          Vertex weight type (default: int)\n");
    printf("  --edge-a <float>              Lower bound for edge weights (default: 1.0)\n");
    printf("  --edge-b <float>              Upper bound for edge weights (default: 10.0)\n");
    printf("  --edge-family <family>        Edge weight family (default: RANDOM)\n");
    printf("  --edge-type <type>            Edge weight type (default: int)\n");
    printf("  --group-penalty-a <float>     Lower bound for group penalties (default: 1.0)\n");
    printf("  --group-penalty-b <float>     Upper bound for group penalties (default: 10.0)\n");
    printf("  --group-penalty-type <type>   Group penalty type (default: int)\n");
    printf("  --group-size <int>            Group size (default: 10.0)\n");
    printf("  --group-size-mode <mode>      Group size mode (default: auto)\n");
    printf("  --group-mode <mode>           If 'disjoint' groups tend to be disjoint, if 'overlap', groups may overlap (default: overlap)\n");
    printf("  --instance-type <type>        Instance type description (default: Node Weighted Group Steiner Tree Instance)\n");
}

int main(int argc, char *argv[]) {
    uint32_t n = 10;       // Number of vertices
    uint64_t m = 30;      // Number of edges
    uint16_t k = 4;           // Number of groups
    uint16_t seed = 12345;     // Random seed
    const char *e = "yes";
    const char *v = "yes";
    const char *p = "no";
    uint32_t group_size = n;

    char filename[256] = "";
    float vertex_a = 1.0, vertex_b = 10.0;
    const char *vertex_weight_type = "int";
    float edge_a = 1.0, edge_b = 10.0;
    const char *edge_weight_family = "RANDOM";
    const char *edge_weight_type = "int";
    float group_penalty_a = 1.0, group_penalty_b = 10.0;
    const char *group_penalty_type = "int";
    char instance_type[256];
    char instance_type_code[256];
    const char *instance_type_per_size = "Group ";
    const char *instance_type_code_per_size = "G";
    const char *group_size_mode = "auto";
    char group_mode[256] = "overlap";
    

    struct option long_options[] = {
        {"vertex-a", required_argument, 0, 0},
        {"vertex-b", required_argument, 0, 0},
        {"vertex-type", required_argument, 0, 0},
        {"edge-a", required_argument, 0, 0},
        {"edge-b", required_argument, 0, 0},
        {"edge-family", required_argument, 0, 0},
        {"edge-type", required_argument, 0, 0},
        {"group-penalty-a", required_argument, 0, 0},
        {"group-penalty-b", required_argument, 0, 0},
        {"group-penalty-type", required_argument, 0, 0},
        {"group-size", required_argument, 0, 0},
        {"group-size-mode", required_argument, 0, 0},
        {"group-mode", required_argument, 0, 0},
        {0, 0, 0, 0}
    };

    int opt;
    int long_index = 0;

    while ((opt = getopt_long(argc, argv, "n:m:k:s:e:v:p:f:", long_options, &long_index)) != -1) {
        switch (opt) {
            case 'n':
                n = (uint32_t)atoi(optarg);
                break;
            case 'm':
                m = (uint64_t)atoll(optarg);
                break;
            case 'k':
                k = (uint16_t)atoi(optarg);
                break;
            case 's':
                seed = (uint16_t)atoi(optarg);
                break;
            case 'e':
                e = optarg;
                break;
            case 'v':
                v = optarg;
                break;
            case 'p':
                p = optarg;
                break;
            case 'f':
                snprintf(filename, sizeof(filename), "%s", optarg);
                break;
            case 0:
                if (strcmp(long_options[long_index].name, "vertex-a") == 0) {
                    vertex_a = atof(optarg);
                } else if (strcmp(long_options[long_index].name, "vertex-b") == 0) {
                    vertex_b = atof(optarg);
                } else if (strcmp(long_options[long_index].name, "vertex-type") == 0) {
                    vertex_weight_type = optarg;
                } else if (strcmp(long_options[long_index].name, "edge-a") == 0) {
                    edge_a = atof(optarg);
                } else if (strcmp(long_options[long_index].name, "edge-b") == 0) {
                    edge_b = atof(optarg);
                } else if (strcmp(long_options[long_index].name, "edge-family") == 0) {
                    edge_weight_family = optarg;
                } else if (strcmp(long_options[long_index].name, "edge-type") == 0) {
                    edge_weight_type = optarg;
                } else if (strcmp(long_options[long_index].name, "group-penalty-a") == 0) {
                    group_penalty_a = atof(optarg);
                } else if (strcmp(long_options[long_index].name, "group-penalty-b") == 0) {
                    group_penalty_b = atof(optarg);
                } else if (strcmp(long_options[long_index].name, "group-penalty-type") == 0) {
                    group_penalty_type = optarg;
                } else if (strcmp(long_options[long_index].name, "group-size") == 0) {
                    group_size = atoi(optarg);
                } else if (strcmp(long_options[long_index].name, "group-size-mode") == 0) {
                    group_size_mode = optarg;
                } else if (strcmp(long_options[long_index].name, "group-mode") == 0) {
                    snprintf(group_mode, sizeof(group_mode), "%s", optarg);
                }
                break;
            default:
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (group_size == 1) {
        instance_type_per_size = "";
        instance_type_code_per_size = "";
    } else if (group_size <= 0 || group_size > n){
        fprintf(stderr, "Group size is: %d. MUST BE at least 1 and at most n=%d.\n", group_size, n);
        exit(EXIT_FAILURE);
    }

    if (strcmp(e,"yes") == 0 && strcmp(v,"yes")==0 && strcmp(p,"yes")==0){
        snprintf(instance_type, sizeof(instance_type), "Prize-Collecting %sSteiner Tree Instance with both edge and node weights", instance_type_per_size);
        snprintf(instance_type_code, sizeof(instance_type_code), "PCNWEW%sST", instance_type_code_per_size);
    }else if (strcmp(e,"yes") == 0 && strcmp(v,"yes")==0 && strcmp(p,"no")==0){
        snprintf(instance_type, sizeof(instance_type), "%sSteiner Tree Instance with both edge and node weights", instance_type_per_size);
        snprintf(instance_type_code, sizeof(instance_type_code), "NWEW%sST", instance_type_code_per_size);
    }else if (strcmp(e,"yes") == 0 && strcmp(v,"no")==0 && strcmp(p,"yes")==0){
        snprintf(instance_type, sizeof(instance_type), "Prize-Collecting %sSteiner Tree Instance", instance_type_per_size);
        snprintf(instance_type_code, sizeof(instance_type_code), "PCEW%sST", instance_type_code_per_size);
    }else if (strcmp(e,"yes") == 0 && strcmp(v,"no")==0 && strcmp(p,"no")==0){
        snprintf(instance_type, sizeof(instance_type), "%sSteiner Tree Instance", instance_type_per_size);
        snprintf(instance_type_code, sizeof(instance_type_code), "EW%sST", instance_type_code_per_size);
    }else if (strcmp(e,"no") == 0 && strcmp(v,"yes")==0 && strcmp(p,"no")==0){
        snprintf(instance_type, sizeof(instance_type), "Node Weighted %sSteiner Tree Instance", instance_type_per_size);
        snprintf(instance_type_code, sizeof(instance_type_code), "NW%sST", instance_type_code_per_size);
    }else if (strcmp(e,"no") == 0 && strcmp(v,"yes")==0 && strcmp(p,"yes")==0){
        snprintf(instance_type, sizeof(instance_type), "Prize-Collecting Node Weighted %sSteiner Tree Instance", instance_type_per_size);
        snprintf(instance_type_code, sizeof(instance_type_code), "PCNW%sST", instance_type_code_per_size);
    }else if (strcmp(e,"no") == 0 && strcmp(v,"no")==0 && strcmp(p,"yes")==0){
        snprintf(instance_type, sizeof(instance_type), "Prize-Collecting Unweighted %sSteiner Tree Instance", instance_type_per_size);
        snprintf(instance_type_code, sizeof(instance_type_code), "PCUW%sST", instance_type_code_per_size);
    }else{
        snprintf(instance_type, sizeof(instance_type), "Unweighted %sSteiner Tree Instance", instance_type_per_size);
        snprintf(instance_type_code, sizeof(instance_type_code), "UW%sST", instance_type_code_per_size);
    }


    


    generate_random_group_steiner_instance(
        n, m, k, e, v, p, filename,
        vertex_a, vertex_b, vertex_weight_type,
        edge_a, edge_b, edge_weight_family, edge_weight_type, group_penalty_a, group_penalty_b, group_penalty_type, &group_size, group_size_mode, group_mode, instance_type, instance_type_code, &seed);

    printf("Random %s written to %s\n",instance_type, filename);
    return 0;
}
