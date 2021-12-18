/*
 * Unordered map
 */

#ifndef MAP_H
#define MAP_H

#include <cdat.h>

/*
 * Generate type specific definitions
 */
#define MAP_GEN(ktype, vtype, khash, kcmp, stru_name, fn_pre)                  \
                                                                               \
typedef struct {                                                               \
    uint8_t state;                                                             \
    size_t hash;                                                               \
    ktype key;                                                                 \
    vtype val;                                                                 \
} stru_name##Bucket;                                                           \
                                                                               \
typedef struct {                                                               \
    size_t active;  /* Number of active buckets */                             \
    size_t load;    /* Number of active + dummy buckets */                     \
    size_t size;    /* Total number of buckets */                              \
    stru_name##Bucket *arr;                                                    \
} stru_name;                                                                   \
                                                                               \
static inline void fn_pre##_init(stru_name *self)                              \
{                                                                              \
    self->active = 0;                                                          \
    self->load = 0;                                                            \
    self->size = 8;     /* Start with a small power of 2 as a size */          \
    self->arr = calloc(self->size, sizeof *self->arr);                         \
}                                                                              \
                                                                               \
static inline void fn_pre##_free(stru_name *self)                              \
{                                                                              \
    free(self->arr);                                                           \
}                                                                              \
                                                                               \
static inline void fn_pre##_rehash(stru_name *self)                            \
{                                                                              \
    size_t oldsize = self->size;                                               \
    stru_name##Bucket *oldarr = self->arr;                                     \
                                                                               \
    self->load = self->active;                  /* Dummies aren't copied */    \
    self->size = round_pow2(self->active * 3);  /* Initial load will be 33% */ \
                                                                               \
    if (!(self->arr = calloc(self->size, sizeof *self->arr)))                  \
        abort();                                                               \
                                                                               \
    for (size_t i = 0; i < oldsize; ++i)                                       \
        if (oldarr[i].state == S_ACTIVE) {                                     \
            size_t perturb = oldarr[i].hash, j = perturb % self->size;         \
            while (self->arr[j].state != S_EMPTY) {                            \
                perturb >>= 5;                                                 \
                j = (j * 5 + perturb + 1) % self->size;                        \
            }                                                                  \
            self->arr[j] = oldarr[i];                                          \
        }                                                                      \
                                                                               \
    free(oldarr);                                                              \
}                                                                              \
                                                                               \
static inline vtype *fn_pre##_put(stru_name *self, ktype key)                  \
{                                                                              \
    if (self->load > self->size * 2 / 3)                                       \
        fn_pre##_rehash(self);                                                 \
                                                                               \
    size_t hash = khash(key), perturb = hash, i = hash % self->size;           \
    ssize_t d = -1;                                                            \
                                                                               \
    for (;;) {                                                                 \
        switch (self->arr[i].state) {                                          \
        case S_EMPTY:                                                          \
            ++self->active;     /* Active bucket count always increases */     \
            if (d < 0)                                                         \
                ++self->load;   /* If we cannot re-use load increases too */   \
            else                                                               \
                i = d;                                                         \
            goto setkey;                                                       \
        case S_DUMMY:                                                          \
            if (d < 0)  /* Save first deleted bucket for re-use */             \
                d = i;                                                         \
            break;                                                             \
        case S_ACTIVE:                                                         \
            if (self->arr[i].hash == hash && !kcmp(self->arr[i].key, key))     \
                goto justval;                                                  \
            break;                                                             \
        }                                                                      \
        perturb >>= 5;                                                         \
        i = (i * 5 + perturb + 1) % self->size;                                \
    }                                                                          \
                                                                               \
setkey:  /* Setup bucket with key */                                           \
    self->arr[i].state = S_ACTIVE;                                             \
    self->arr[i].hash = hash;                                                  \
    self->arr[i].key = key;                                                    \
justval: /* Return pointer to value */                                         \
    return &self->arr[i].val;                                                  \
}                                                                              \
                                                                               \
static inline vtype *fn_pre##_get(stru_name *self, ktype key)                  \
{                                                                              \
    size_t hash = khash(key), perturb = hash, i = hash % self->size;           \
                                                                               \
    while (self->arr[i].state != S_EMPTY) {                                    \
        if (self->arr[i].state == S_ACTIVE                                     \
                && self->arr[i].hash == hash && !kcmp(self->arr[i].key, key))  \
            return &self->arr[i].val;                                          \
        perturb >>= 5;                                                         \
        i = (i * 5 + perturb + 1) % self->size;                                \
    }                                                                          \
    return NULL;                                                               \
}                                                                              \
                                                                               \
static inline _Bool fn_pre##_del(stru_name *self, ktype key)                   \
{                                                                              \
    size_t hash = khash(key), perturb = hash, i = hash % self->size;           \
                                                                               \
    while (self->arr[i].state != S_EMPTY) {                                    \
        if (self->arr[i].state == S_ACTIVE                                     \
                && self->arr[i].hash == hash && !kcmp(self->arr[i].key, key)) {\
            self->arr[i].state = S_DUMMY;                                      \
            --self->active;                                                    \
            return 1;                                                          \
        }                                                                      \
        perturb >>= 5;                                                         \
        i = (i * 5 + perturb + 1) % self->size;                                \
    }                                                                          \
    return 0;                                                                  \
}                                                                              \

#endif
