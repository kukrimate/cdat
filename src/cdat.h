/*
 * Common helpers
 */

#ifndef DAT_H
#define DAT_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Determine the number of element in an array */
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

/* Round to the nearest power of 2 >= to size */
static inline size_t round_pow2(size_t size)
{
    size_t newsize = 1;
    while (newsize < size)
        newsize <<= 1;
    return newsize;
}

/* Identitiy function for "hashing integers" */
static inline int inthash(int x)
{
    return x;
}

/* Strcmp-like comparison function for integers */
static inline int intcmp(int a, int b)
{
    if (a < b)
        return -1;
    else if (a > b)
        return 1;
    else
        return 0;
}

/* Hash function for strings, implements djb2 */
static inline uint32_t strhash(const char *str)
{
	uint32_t hash = 5381;
	for (; *str; ++str)
		hash = (hash << 5) + hash + *str; // Same as: 33 * hash + *str
	return hash;
}

#endif
