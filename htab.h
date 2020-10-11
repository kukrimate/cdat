/*
 * Generic hash table
 * Copyright (C) Mate Kurki, 2020
 */
#ifndef HTAB_H
#define HTAB_H

/*
 * Number of buckets to start with
 */
#define HTAB_START_BUCKETS 16

/*
 * Calculate the max allowed number of filled buckets
 */
#define HTAB_LOAD_LIMIT(n) (n * 3 / 4)

/*
 * Factor to grow the bucket count by when the load limit is reached
 */
#define HTAB_GROW_FACTOR 2

/*
 * Generate type specific definitions
 */
#define htab_gen(T, U, hash, cmp, A) \
\
struct helem##A { \
	T key; \
	U value; \
}; \
\
struct htab##A { \
	size_t load; \
	size_t size; \
	struct helem##A *arr; \
}; \
\
static inline void htab_alloc##A(struct htab##A *self) \
{ \
	self->load = 0; \
	self->size = HTAB_START_BUCKETS; \
	self->arr = calloc(self->size, sizeof(struct helem##A)); \
} \
\
static inline void htab_free##A(struct htab##A *self) \
{ \
	free(self->arr); \
} \
\
static inline void htab_put##A(struct htab##A *self, T key, U val) \
{ \
	size_t i; \
\
	size_t oldsize; \
	struct helem##A *oldarr; \
\
	for (i = hash(key) % self->size; \
			self->arr[i].key; \
			i = (i + 1) % self->size) \
		if (cmp(self->arr[i].key, key)) \
			break; \
\
	self->arr[i].key = key; \
	self->arr[i].value = val; \
\
	if (++self->load >= HTAB_LOAD_LIMIT(self->size)) { \
		oldsize = self->size; \
		oldarr = self->arr; \
\
		self->size *= HTAB_GROW_FACTOR; \
		self->arr = calloc(self->size, sizeof(struct helem##A)); \
\
		for (i = 0; i < oldsize; ++i) \
			if (oldarr[i].key) \
				htab_put##A(self, \
					oldarr[i].key, oldarr[i].value); \
\
		free(oldarr); \
	} \
} \
\
static inline U htab_get##A(struct htab##A *self, T key) \
{ \
	size_t i; \
	for (i = hash(key) % self->size; \
			self->arr[i].key; \
			i = (i + 1) % self->size) \
		if (cmp(self->arr[i].key, key)) \
			return self->arr[i].value; \
	return NULL; \
}

/*
 * Macros for nicer syntax
 */
#define htab(x)       htab##x
#define htab_alloc(x) htab_alloc##x
#define htab_free(x)  htab_free##x
#define htab_put(x)   htab_put##x
#define htab_get(x)   htab_get##x

/*
 * Pre-defined hash and compare functions for strings
 */
static inline size_t str_hash(char *str)
{
	size_t r;

	r = 5381;
	for (; *str; ++str)
		r = (r << 15) + r + *str;
	return r;
}

static inline int str_cmp(char *str1, char *str2)
{
	return !strcmp(str1, str2);
}

#endif
