#ifndef DAT_H
#define DAT_H

static inline int inthash(int x)
{
    return x;
}

static inline int intcmp(int a, int b)
{
    if (a < b)
        return -1;
    else if (a > b)
        return 1;
    else
        return 0;
}

static inline uint32_t strhash(const char *str)
{
	uint32_t hash = 5381;
	for (; *str; ++str)
		hash = (hash << 5) + hash + *str; // Same as: 33 * hash + *str
	return hash;
}

#endif
