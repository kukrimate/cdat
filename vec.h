/*
 * Vector, aka dynamically growing array
 */
#ifndef VEC_H
#define VEC_H

/*
 * Pre-allocated capacity
 */
#define VEC_PREALLOC 10

/*
 * Grow factor when full
 */
#define VEC_GROW_FACTOR 2

/*
 * Generate type specific definitions
 */
#define VEC_GEN(type, prefix) \
\
typedef struct { \
	size_t n;    /* Number of elements */ \
	size_t size; /* Size of the arr */ \
	type   *arr; /* The backing arr */ \
} prefix##vec; \
\
static inline void \
prefix##vec_init(prefix##vec *self) \
{ \
	self->n = 0; \
	self->size = VEC_PREALLOC; \
	self->arr = reallocarray(NULL, self->size, sizeof(type)); \
} \
\
static inline void \
prefix##vec_free(prefix##vec *self) \
{ \
	free(self->arr); \
} \
\
static inline void \
prefix##vec_reserve(prefix##vec *self, size_t size) \
{ \
	self->size = size; \
	self->arr = reallocarray(self->arr, self->size, sizeof(type)); \
} \
\
static inline void \
prefix##vec_add(prefix##vec *self, type m) \
{ \
	if (++self->n > self->size) \
		prefix##vec_reserve(self, self->n * VEC_GROW_FACTOR); \
	self->arr[self->n - 1] = m; \
} \
\
static inline void \
prefix##vec_addall(prefix##vec *self, type *m, size_t n) \
{ \
	self->n += n; \
	if (self->n > self->size) \
		prefix##vec_reserve(self, self->n * VEC_GROW_FACTOR); \
	memcpy(self->arr + self->n - n, m, n * sizeof(type)); \
} \
\
static inline type * \
prefix##vec_push(prefix##vec *self) \
{ \
	if (++self->n > self->size) \
		prefix##vec_reserve(self, self->n * VEC_GROW_FACTOR); \
	return self->arr + self->n - 1; \
}


#endif
