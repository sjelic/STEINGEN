#ifndef RAND_R_LARGE_H
#define RAND_R_LARGE_H

#include <stdint.h>


/**
 * Generates a large random number by combining two `rand_r` calls.
 *
 * @param seed A pointer to an unsigned 16-bit seed used for the random number generator.
 *             The seed is updated after each call to maintain the sequence.
 *
 * @return A 64-bit unsigned random number.
 */
uint64_t rand_r_large(uint16_t* seed);

#endif // RAND_R_LARGE_H
