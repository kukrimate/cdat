/*
 * Generic, dynamically growing array
 *  Copyright (C) Mate Kurki, 2020
 *  Part of libkm, released under the ISC license
 */
#ifndef DYNARR_H
#define DYNARR_H

/*
 * Pre-allocated capacity
 */
#define DYNARR_PREALLOC 10

/*
 * Grow factor when full
 */
#define DYNARR_GROW_FACTOR 2

/*
 * Generate type specific definitons
 */
#define dynarr_gen(T, A) \
\
struct dynarr##A { \
	size_t nmemb; \
	size_t avail; \
	T *mem; \
}; \
\
static inline void dynarr##A##_alloc(struct dynarr##A *self) \
{ \
	self->nmemb = 0; \
	self->avail = DYNARR_PREALLOC; \
	self->mem = reallocarray(NULL, self->avail, sizeof(T)); \
} \
\
static inline void dynarr##A##_free(struct dynarr##A *self) \
{ \
	free(self->mem); \
} \
\
static inline void dynarr##A##_clear(struct dynarr##A *self) \
{ \
	self->nmemb = 0; \
} \
\
static inline void dynarr##A##_reserve(struct dynarr##A *self, size_t avail) \
{ \
	self->avail = avail; \
	self->mem = reallocarray(self->mem, self->avail, sizeof(T)); \
} \
\
static inline void dynarr##A##_add(struct dynarr##A *self, T m) \
{ \
	if (++self->nmemb > self->avail) \
		dynarr##A##_reserve(self, self->nmemb * DYNARR_GROW_FACTOR); \
	((T *) self->mem)[self->nmemb - 1] = m; \
} \
\
static inline T dynarr##A##_get(struct dynarr##A *self, size_t i) \
{ \
	return ((T *) self->mem)[i]; \
} \
static inline T *dynarr##A##_ptr(struct dynarr##A *self, size_t i) \
{ \
	return (T *) self->mem + i; \
}

#endif
