/*
 * Unordered map
 */
#ifndef MAP_H
#define MAP_H

/*
 * Number of preallocated buckets
 */
#ifndef MAP_PREALLOC
#define MAP_PREALLOC 10
#endif

/*
 * Rehash the table if the load is over this number
 */
#ifndef MAP_REHASH_TRESHOLD
#define MAP_REHASH_TRESHOLD(size) (size * 3 / 4)
#endif

/*
 * Grow the table if the load would be over this number after a rehash
 */
#ifndef MAP_GROW_TRESHOLD
#define MAP_GROW_TRESHOLD(size) (size / 2)
#endif

/*
 * Multiplier for the size when a grow is triggered
 */
#ifndef MAP_GROW_FACTOR
#define MAP_GROW_FACTOR 2
#endif

/*
 * Generate type specific definitions
 */
#define MAP_GEN(ktype, vtype, khash, kcmp, prefix) \
\
typedef struct { \
	_Bool present; \
	_Bool deleted; \
	ktype key; \
	vtype val; \
} prefix##elem; \
\
typedef struct { \
	size_t present_cnt; \
	size_t deleted_cnt; \
	size_t size; \
	prefix##elem *arr; \
} prefix##map; \
\
static inline void \
prefix##map_init(prefix##map *self) \
{ \
	self->present_cnt = 0; \
	self->deleted_cnt = 0; \
	self->size = MAP_PREALLOC; \
	self->arr = calloc(self->size, sizeof(*self->arr)); \
} \
\
static inline void \
prefix##map_free(prefix##map *self) \
{ \
	free(self->arr); \
} \
\
static inline void \
prefix##map_put(prefix##map *self, ktype key, vtype val); \
\
static inline void \
prefix##map_rehash(prefix##map *self) \
{ \
	size_t i, oldsize; \
	prefix##elem *oldarr; \
\
	/* Save old bucket array for rehashing */ \
	oldsize = self->size; \
	oldarr = self->arr; \
\
	/* Check if removing deleted pairs is enough or do we need to grow */ \
	if (self->present_cnt - self->deleted_cnt >= MAP_GROW_TRESHOLD(self->size)) \
		self->size *= MAP_GROW_FACTOR; \
	/* Set deleted and present count to 0 initially for the new table */ \
	self->present_cnt = 0; \
	self->deleted_cnt = 0; \
	/* Allocate new bucket array */ \
	self->arr = calloc(self->size, sizeof(*self->arr)); \
\
	/* Put all non-deleted pairs into the new table */ \
	for (i = 0; i < oldsize; ++i) \
		if (oldarr[i].present && !oldarr[i].deleted)\
			prefix##map_put(self, oldarr[i].key, oldarr[i].val); \
\
	free(oldarr); \
} \
\
void  \
prefix##map_put(prefix##map *self, ktype key, vtype val) \
{ \
	size_t i; \
\
	/* Probe for the key (even if deleted) or an empty bucket */ \
	for (i = khash(key) % self->size; \
			self->arr[i].present; i = (i + 1) % self->size) \
		if (kcmp(self->arr[i].key, key)) \
			break; \
\
	/* Increase load if the key was not present in the table before */ \
	if (!self->arr[i].present) \
		++self->present_cnt; \
	/* Decrease deleted count if re-filling a deleted pair's bucket */ \
	else if (self->arr[i].deleted) \
		--self->deleted_cnt; \
\
	/* Fill bucket with the pair */ \
	self->arr[i].present = 1; \
	self->arr[i].deleted = 0; \
	self->arr[i].key = key; \
	self->arr[i].val = val; \
\
	/* Check if we reached the treshold for a rehash */ \
	if (self->present_cnt >= MAP_REHASH_TRESHOLD(self->size)) \
		prefix##map_rehash(self); \
} \
\
static inline void \
prefix##map_del(prefix##map *self, ktype key) \
{ \
	size_t i; \
\
	for (i = khash(key) % self->size; \
			self->arr[i].present; i = (i + 1) % self->size) \
		if (kcmp(self->arr[i].key, key)) { \
			/* Avoid double counting if the pair was already deleted */ \
			if (self->arr[i].deleted) \
				return; \
			/* Mark pair as deleted if it wasn't already marked */ \
			self->arr[i].deleted = 1; \
			++self->deleted_cnt; \
			return; \
		} \
} \
\
static inline _Bool \
prefix##map_get(prefix##map *self, ktype key, vtype *val) \
{ \
	size_t i; \
\
	for (i = khash(key) % self->size; \
			self->arr[i].present; i = (i + 1) % self->size) \
		if (kcmp(self->arr[i].key, key)) { \
			/* Signal not-present if the pair was deleted */ \
			if (self->arr[i].deleted) \
				return 0; \
			/* Copy value to out pointer and return present */ \
			*val = self->arr[i].val; \
			return 1; \
		} \
	/* Signal not-present if the key wasn't found */ \
	return 0; \
}

/*
 * Hash and compare for scaler values
 */
#define IHASH(x) (x)
#define ICOMPARE(x, y) ((x) == (y))

#endif
