#ifndef RANDOM_GROUPS_H
#define RANDOM_GROUPS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// Struct for representing groups
typedef struct {
    uint32_t *group; // Pointer to an array of group members
    uint32_t size;   // Number of members in the group
} Group;

// Function to generate random groups
void shuffle(uint32_t *array, uint32_t size, uint16_t* seed);
void generate_random_groups(uint32_t n, uint16_t k, Group *groups, uint32_t* group_size, const char* group_size_mode, char* groups_disjoint, uint16_t* seed);

#endif // RANDOM_GROUPS_H
