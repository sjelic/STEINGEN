#include <stdio.h>
#include <stdlib.h>
#include "random_graph.h"
#include "random_groups.h"

int main() {
    unsigned int n = 20, m = 80, k = 4, seed = 5;
    Edge *edges = malloc(m * sizeof(Edge));
    if (!edges) {
        fprintf(stderr, "Error: Memory allocation of EDGE array: edges FAILED.\n");
        exit(EXIT_FAILURE);
    }

    random_spanning_tree(n, m, seed, edges);

    printf("Generated graph edges:\n");
    for (unsigned int i = 0; i < m; i++) {
        printf("Edge %d -> %d\n", edges[i].src, edges[i].dest);
    }


    Group *groups;
    generate_random_groups(n, k, &groups);

    // Print groups
    printf("Random groups:\n");
    for (unsigned int i = 0; i < k; i++) {
        printf("Group %u: ", i);
        for (unsigned int j = 0; j < groups[i].size; j++) {
            printf("%u ", groups[i].group[j]);
        }
        printf("\n");
        free(groups[i].group); // Free each group's array
    }
    free(groups); // Free the groups array
    free(edges);
    return 0;
}
