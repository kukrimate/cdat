/*
 * Dynamically growing array
 * Copyright (C) Mate Kurki, 2020
 * Part of libkm, released under the ISC license
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
 * Generate type specific definitions
 */
#define dynarr_gen(T, A) \
\
struct dynarr##A { \
	size_t nmemb; \
	size_t avail; \
	T *array; \
}; \
\
static inline void dynarr_alloc##A(struct dynarr##A *self) \
{ \
	self->nmemb = 0; \
	self->avail = DYNARR_PREALLOC; \
	self->array = reallocarray(NULL, self->avail, sizeof(T)); \
} \
\
static inline void dynarr_free##A(struct dynarr##A *self) \
{ \
	free(self->array); \
} \
\
static inline void dynarr_reserve##A(struct dynarr##A *self, size_t avail) \
{ \
	self->avail = avail; \
	self->array = reallocarray(self->array, self->avail, sizeof(T)); \
} \
\
static inline void dynarr_add##A(struct dynarr##A *self, T m) \
{ \
	if (++self->nmemb > self->avail) \
		dynarr_reserve##A(self, self->nmemb * DYNARR_GROW_FACTOR); \
	self->array[self->nmemb - 1] = m; \
}

/*
 * Macros for nicer syntax
 */
#define dynarr(x)         dynarr##x
#define dynarr_alloc(x)   dynarr_alloc##x
#define dynarr_free(x)    dynarr_free##x
#define dynarr_reserve(x) dynarr_reserve##x
#define dynarr_add(x)     dynarr_add##x

#endif
