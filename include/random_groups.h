#ifndef RANDOM_GROUPS_H
#define RANDOM_GROUPS_H

#include <stdlib.h>
#include <stdio.h>

// Struct for representing groups
typedef struct {
    unsigned int *group; // Pointer to an array of group members
    unsigned int size;   // Number of members in the group
} Group;

// Function to generate random groups
void generate_random_groups(unsigned int n, unsigned int k, Group **groups);

#endif // RANDOM_GROUPS_H
