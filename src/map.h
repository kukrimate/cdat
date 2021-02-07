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
#define MAP_GEN(ktype, vtype, khash, kcmp, alias) \
\
typedef struct { \
	_Bool present; \
	_Bool deleted; \
	ktype key; \
	vtype val; \
} MAP_ELEM##alias; \
\
typedef struct { \
	size_t present_cnt; \
	size_t deleted_cnt; \
	size_t size; \
	MAP_ELEM##alias *arr; \
} MAP##alias; \
\
static inline void MAP##alias##_init(MAP##alias *self) \
{ \
	self->present_cnt = 0; \
	self->deleted_cnt = 0; \
	self->size = MAP_PREALLOC; \
	self->arr = calloc(self->size, sizeof(*self->arr)); \
} \
\
static inline void MAP##alias##_free(MAP##alias *self) \
{ \
	free(self->arr); \
} \
\
static inline void MAP##alias##_put(MAP##alias *self, ktype key, vtype val); \
\
static inline void MAP##alias##_rehash(MAP##alias *self) \
{ \
	size_t i, oldsize; \
	MAP_ELEM##alias *oldarr; \
\
	/* Save old bucket array for rehashing */ \
	oldsize = self->size; \
	oldarr = self->arr; \
\
	/* Check if removing deleted pairs is enough or do we need to grow */ \
	if (self->present_cnt - self->deleted_cnt > MAP_GROW_TRESHOLD(self->size)) \
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
			MAP##alias##_put(self, oldarr[i].key, oldarr[i].val); \
\
	free(oldarr); \
} \
\
static inline vtype *MAP##alias##_putptr(MAP##alias *self, ktype key) \
{ \
	size_t i; \
	\
	/* Check if a rehash is needed before inserting a new pair */ \
	if (self->present_cnt > MAP_REHASH_TRESHOLD(self->size)) \
		MAP##alias##_rehash(self); \
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
	return &self->arr[i].val; \
} \
\
static inline void MAP##alias##_put(MAP##alias *self, ktype key, vtype val)  \
{ \
	*MAP##alias##_putptr(self, key) = val; \
} \
\
static inline void MAP##alias##_del(MAP##alias *self, ktype key) \
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
static inline vtype *MAP##alias##_getptr(MAP##alias *self, ktype key) \
{ \
	size_t i; \
\
	for (i = khash(key) % self->size; \
			self->arr[i].present; i = (i + 1) % self->size) \
		if (kcmp(self->arr[i].key, key)) { \
			/* Signal not-present if the pair was deleted */ \
			if (self->arr[i].deleted) \
				return NULL; \
			/* Return pointer to value */ \
			return &self->arr[i].val; \
		} \
	/* Signal not-present if key was found */ \
	return NULL; \
} \
\
static inline _Bool MAP##alias##_get(MAP##alias *self, ktype key, vtype *val) \
{ \
	vtype *ptr = MAP##alias##_getptr(self, key); \
	if (ptr) { \
		*val = *ptr; \
		return 1; \
	} \
	return 0; \
}

/*
 * Hash and compare for scaler values
 */
#define IHASH(x) (x)
#define ICOMPARE(x, y) ((x) == (y))

#endif
