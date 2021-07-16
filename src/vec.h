/*
 * Vector, aka dynamically growing array
 */

#ifndef VEC_H
#define VEC_H

#include <cdat.h>

/*
 * Minimum size
 */
#ifndef VEC_MINSIZE
#define VEC_MINSIZE 8
#endif

/*
 * Grow factor when full
 */
#ifndef VEC_GROW_FACTOR
#define VEC_GROW_FACTOR 2
#endif

/*
 * Generate type specific definitions
 */
#define VEC_GEN(type, stru_name, fn_pre)                                       \
                                                                               \
typedef struct {                                                               \
    size_t n;    /* Number of elements */                                      \
    size_t size; /* Size of array */                                           \
    type *arr;   /* Backing array */                                           \
} stru_name;                                                                   \
                                                                               \
static inline void fn_pre##_init(stru_name *self)                              \
{                                                                              \
    self->n = 0;                                                               \
    self->size = VEC_MINSIZE;                                                  \
    self->arr = reallocarray(NULL, self->size, sizeof(type));                  \
}                                                                              \
                                                                               \
static inline void fn_pre##_free(stru_name *self)                              \
{                                                                              \
    free(self->arr);                                                           \
}                                                                              \
                                                                               \
static inline void fn_pre##_reserve(stru_name *self, size_t size)              \
{                                                                              \
    self->size = size;                                                         \
    self->arr = reallocarray(self->arr, self->size, sizeof(type));             \
}                                                                              \
                                                                               \
static inline void fn_pre##_add(stru_name *self, type m)                       \
{                                                                              \
    if (++self->n > self->size)                                                \
        fn_pre##_reserve(self, self->n * VEC_GROW_FACTOR);                     \
    self->arr[self->n - 1] = m;                                                \
}                                                                              \
                                                                               \
static inline void fn_pre##_addall(stru_name *self, type const *m, size_t n)   \
{                                                                              \
    self->n += n;                                                              \
    if (self->n > self->size)                                                  \
        fn_pre##_reserve(self, self->n * VEC_GROW_FACTOR);                     \
    memcpy(self->arr + self->n - n, m, n * sizeof(type));                      \
}                                                                              \
                                                                               \
static inline void fn_pre##_extend(stru_name *self, stru_name *other)          \
{                                                                              \
    fn_pre##_addall(self, other->arr, other->n);                               \
}                                                                              \
                                                                               \
static inline type *fn_pre##_push(stru_name *self)                             \
{                                                                              \
    if (++self->n > self->size)                                                \
        fn_pre##_reserve(self, self->n * VEC_GROW_FACTOR);                     \
    return self->arr + self->n - 1;                                            \
}                                                                              \
                                                                               \
static inline type *fn_pre##_at(stru_name *self, size_t i)                     \
{                                                                              \
    return self->arr + i;                                                      \
}                                                                              \
                                                                               \
static inline type *fn_pre##_top(stru_name *self)                              \
{                                                                              \
    return self->arr + self->n - 1;                                            \
}                                                                              \
                                                                               \
static inline type fn_pre##_pop(stru_name *self)                               \
{                                                                              \
    return self->arr[--self->n];                                               \
}                                                                              \
                                                                               \
//
// String builder (character vector) type
//

VEC_GEN(char, StringBuilder, sb)

static inline char *sb_str(StringBuilder *self)
{
    sb_add(self, 0);
    return self->arr;
}

static inline void sb_addstr(StringBuilder *self, const char *str)
{
    sb_addall(self, str, strlen(str));
}

#endif
