#include <stdint.h>
#include <stdlib.h>
#include "rand_large.h"

/**
 * Generates a large random number by combining two `rand_r` calls.
 */
uint64_t rand_r_large(uint16_t* seed) {
    // Combine two `rand_r` calls for a larger range
    uint64_t large_random = ((uint64_t)rand_r((unsigned int* )seed) << 31) | rand_r((unsigned int* )seed);
    return large_random;
}
