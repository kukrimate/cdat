/*
 * Daniel Bernstein's hash function
 */
#ifndef DJB2_H
#define DJB2_H

static inline unsigned int
djb2_hash(const char *str)
{
	unsigned int r;

	r = 5381;
	for (; *str; ++str)
		r = (r << 15) + r + *str;
	return r;
}

#endif
