/*
 * Unordered set
 */

#ifndef SET_H
#define SET_H

/*
 * Number of preallocated buckets
 */
#ifndef SET_PREALLOC
#define SET_PREALLOC 10
#endif

/*
 * Rehash the table if the load is over this number
 */
#ifndef SET_REHASH_TRESHOLD
#define SET_REHASH_TRESHOLD(size) (size * 3 / 4)
#endif

/*
 * Grow the table if the load would be over this number after a rehash
 */
#ifndef SET_GROW_TRESHOLD
#define SET_GROW_TRESHOLD(size) (size / 2)
#endif

/*
 * Multiplier for the size when a grow is triggered
 */
#ifndef SET_GROW_FACTOR
#define SET_GROW_FACTOR 2
#endif

/*
 * Generate type specific definitions
 */
#define SET_GEN(type, hash, cmp, alias) \
\
typedef struct { \
	_Bool present; \
	_Bool deleted; \
	type key; \
} SET_ELEM##alias; \
\
typedef struct { \
	size_t present_cnt; \
	size_t deleted_cnt; \
	size_t size; \
	SET_ELEM##alias *arr; \
} SET##alias; \
\
static inline void SET##alias##_init(SET##alias *self) \
{ \
	self->present_cnt = 0; \
	self->deleted_cnt = 0; \
	self->size = SET_PREALLOC; \
	self->arr = calloc(self->size, sizeof(*self->arr)); \
} \
\
static inline void SET##alias##_free(SET##alias *self) \
{ \
	free(self->arr); \
} \
\
static inline void SET##alias##_set(SET##alias *self, type key) \
{ \
	size_t i, oldsize; \
	SET_ELEM##alias *oldarr; \
\
	for (i = hash(key) % self->size; \
			self->arr[i].present; i = (i + 1) % self->size) \
		if (cmp(self->arr[i].key, key)) \
			break; \
\
	if (!self->arr[i].present) \
		++self->present_cnt; \
	else if (self->arr[i].deleted) \
		--self->deleted_cnt; \
\
	self->arr[i].present = 1; \
	self->arr[i].deleted = 0; \
	self->arr[i].key = key; \
\
	if (self->present_cnt > SET_REHASH_TRESHOLD(self->size)) { \
		oldsize = self->size; \
		oldarr = self->arr; \
\
		if (self->present_cnt - self->deleted_cnt > SET_GROW_TRESHOLD(self->size)) \
			self->size *= SET_GROW_FACTOR; \
		self->present_cnt = 0; \
		self->deleted_cnt = 0; \
		self->arr = calloc(self->size, sizeof(*self->arr)); \
\
		for (i = 0; i < oldsize; ++i) \
			if (oldarr[i].present && !oldarr[i].deleted) \
				SET##alias##_set(self, oldarr[i].key); \
\
		free(oldarr); \
	} \
} \
static inline void SET##alias##_unset(SET##alias *self, type key) \
{ \
	size_t i; \
	for (i = hash(key) % self->size; \
			self->arr[i].present; i = (i + 1) % self->size) \
		if (cmp(self->arr[i].key, key)) { \
			if (self->arr[i].deleted) \
				return; \
			self->arr[i].deleted = 1; \
			++self->deleted_cnt; \
		} \
} \
\
static inline _Bool SET##alias##_isset(SET##alias *self, type key) \
{ \
	size_t i; \
	for (i = hash(key) % self->size; \
			self->arr[i].present; i = (i + 1) % self->size) \
		if (cmp(self->arr[i].key, key))\
			return !self->arr[i].deleted; \
	return 0; \
}

#endif
