#include <stdio.h>
#include <stdlib.h>
#include "random_graph.h"

int main() {
    unsigned int n = 60000, m = 150000, seed = 0;
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

    free(edges);
    return 0;
}
