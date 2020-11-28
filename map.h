/*
 * Unordered map
 */
#ifndef MAP_H
#define MAP_H

/*
 * Number of preallocated buckets
 */
#define MAP_PREALLOC 10

/*
 * Calculate the max allowed number of filled buckets
 */
#define MAP_LOAD_LIMIT(n) (n * 3 / 4)

/*
 * Factor to grow the bucket count by when the load limit is reached
 */
#define MAP_GROW_FACTOR 2

/*
 * Generate type specific definitions
 */
#define map_gen(ktype, vtype, khash, kcmp, prefix) \
\
struct prefix##elem { \
	ktype key; \
	vtype val; \
}; \
\
struct prefix##map { \
	size_t load; \
	size_t size; \
	struct prefix##elem *arr; \
}; \
\
static inline void \
prefix##map_init(struct prefix##map *self) \
{ \
	self->load = 0; \
	self->size = MAP_PREALLOC; \
	self->arr = calloc(self->size, sizeof(struct prefix##elem)); \
} \
\
static inline void \
prefix##map_free(struct prefix##map *self) \
{ \
	free(self->arr); \
} \
\
static inline void \
prefix##map_put(struct prefix##map *self, ktype key, vtype val) \
{ \
	size_t i; \
\
	size_t oldsize; \
	struct prefix##elem *oldarr; \
\
	for (i = khash(key) % self->size; \
			self->arr[i].key; \
			i = (i + 1) % self->size) \
		if (kcmp(self->arr[i].key, key)) \
			break; \
\
	self->arr[i].key = key; \
	self->arr[i].val = val; \
\
	if (++self->load >= MAP_LOAD_LIMIT(self->size)) { \
		oldsize = self->size; \
		oldarr = self->arr; \
\
		self->size *= MAP_GROW_FACTOR; \
		self->arr = calloc(self->size, sizeof(struct prefix##elem)); \
\
		for (i = 0; i < oldsize; ++i) \
			if (oldarr[i].key) \
				prefix##map_put(self, \
					oldarr[i].key, \
					oldarr[i].val); \
\
		free(oldarr); \
	} \
} \
\
static inline vtype \
prefix##map_get(struct prefix##map *self, ktype key) \
{ \
	size_t i; \
	for (i = khash(key) % self->size; \
			self->arr[i].key; \
			i = (i + 1) % self->size) \
		if (kcmp(self->arr[i].key, key)) \
			return self->arr[i].val; \
	return (vtype) 0; \
}

/*
 * Hash and compare for scaler values
 */
#define shash(x)   (x)
#define scmp(x, y) (x == y)

#endif
