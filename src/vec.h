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
#define VEC_GEN(type, alias) \
\
typedef struct { \
	size_t n;    /* Number of elements */ \
	size_t size; /* Size of the arr */ \
	type   *arr; /* The backing arr */ \
} VEC##alias; \
\
static inline void VEC##alias##_init(VEC##alias *self) \
{ \
	self->n = 0; \
	self->size = VEC_PREALLOC; \
	self->arr = reallocarray(NULL, self->size, sizeof(type)); \
} \
\
static inline void VEC##alias##_free(VEC##alias *self) \
{ \
	free(self->arr); \
} \
\
static inline void VEC##alias##_reserve(VEC##alias *self, size_t size) \
{ \
	self->size = size; \
	self->arr = reallocarray(self->arr, self->size, sizeof(type)); \
} \
\
static inline void VEC##alias##_add(VEC##alias *self, type m) \
{ \
	if (++self->n > self->size) \
		VEC##alias##_reserve(self, self->n * VEC_GROW_FACTOR); \
	self->arr[self->n - 1] = m; \
} \
\
static inline void VEC##alias##_addall(VEC##alias *self, type *m, size_t n) \
{ \
	self->n += n; \
	if (self->n > self->size) \
		VEC##alias##_reserve(self, self->n * VEC_GROW_FACTOR); \
	memcpy(self->arr + self->n - n, m, n * sizeof(type)); \
} \
\
static inline type *VEC##alias##_push(VEC##alias *self) \
{ \
	if (++self->n > self->size) \
		VEC##alias##_reserve(self, self->n * VEC_GROW_FACTOR); \
	return self->arr + self->n - 1; \
}


#endif
