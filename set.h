/*
 * Unordered set
 */
#ifndef SET_H
#define SET_H

/*
 * Number of preallocated buckets
 */
#define SET_PREALLOC 10

/*
 * Calculate the max allowed number of filled buckets
 */
#define SET_LOAD_LIMIT(n) (n * 3 / 4)

/*
 * Factor to grow the bucket count by when the load limit is reached
 */
#define SET_GROW_FACTOR 2

/*
 * Generate type specific definitions
 */
#define SET_GEN(type, hash, cmp, prefix) \
\
typedef struct { \
	size_t load; \
	size_t size; \
	type   *arr; \
} prefix##set; \
\
static inline void \
prefix##set_init(prefix##set *self) \
{ \
	self->load = 0; \
	self->size = SET_PREALLOC; \
	self->arr = calloc(self->size, sizeof(type)); \
} \
\
static inline void \
prefix##set_free(prefix##set *self) \
{ \
	free(self->arr); \
} \
\
static inline void \
prefix##set_set(prefix##set *self, type key) \
{ \
	size_t i; \
\
	size_t oldsize; \
	type *oldarr; \
\
	for (i = hash(key) % self->size; \
			self->arr[i]; \
			i = (i + 1) % self->size) \
		if (cmp(self->arr[i], key)) \
			break; \
\
	self->arr[i] = key; \
\
	if (++self->load >= SET_LOAD_LIMIT(self->size)) { \
		oldsize = self->size; \
		oldarr = self->arr; \
\
		self->size *= SET_GROW_FACTOR; \
		self->arr = calloc(self->size, sizeof(type)); \
\
		for (i = 0; i < oldsize; ++i) \
			if (oldarr[i]) \
				prefix##set_set(self, oldarr[i]); \
\
		free(oldarr); \
	} \
} \
\
static inline _Bool \
prefix##set_isset(prefix##set *self, type key) \
{ \
	size_t i; \
	for (i = hash(key) % self->size; \
			self->arr[i]; \
			i = (i + 1) % self->size) \
		if (cmp(self->arr[i], key)) \
			return 1; \
	return 0; \
}

#endif
