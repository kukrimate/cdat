/*
 * Unordered map
 */

#ifndef MAP_H
#define MAP_H

#include <cdat.h>

/* Minimum number of buckets */
#ifndef MAP_MINSIZE
#define MAP_MINSIZE 8
#endif

/* Maximum number of used buckets allowed for a given table */
#ifndef MAP_RESIZE_TRESHOLD
#define MAP_RESIZE_TRESHOLD(size) (size * 2 / 3)
#endif

/* The new table size will be round_pow2(self->used * MAP_RESIZE_FACTOR) */
#ifndef MAP_RESIZE_FACTOR
#define MAP_RESIZE_FACTOR 3
#endif

/* State of a bucket */
typedef enum {
    MBS_EMPTY,  // Empty
    MBS_DUMMY,  // Previously active
    MBS_ACTIVE, // Currently active
} MapBucketState;

/*
 * Generate type specific definitions
 */
#define MAP_GEN(ktype, vtype, khash, kcmp, stru_name, fn_pre)                  \
                                                                               \
typedef struct {                                                               \
    MapBucketState state;                                                      \
    size_t hash;                                                               \
    ktype key;                                                                 \
    vtype val;                                                                 \
} stru_name##Bucket;                                                           \
                                                                               \
typedef struct {                                                               \
    size_t active_cnt;                                                         \
    size_t size;                                                               \
    stru_name##Bucket *arr;                                                    \
} stru_name;                                                                   \
                                                                               \
static inline void fn_pre##_init(stru_name *self)                              \
{                                                                              \
    self->active_cnt = 0;                                                      \
    self->size = MAP_MINSIZE;                                                  \
    self->arr = calloc(self->size, sizeof *self->arr);                         \
}                                                                              \
                                                                               \
static inline void fn_pre##_free(stru_name *self)                              \
{                                                                              \
    free(self->arr);                                                           \
}                                                                              \
                                                                               \
static inline void fn_pre##_resize(stru_name *self)                            \
{                                                                              \
    size_t oldsize = self->size;                                               \
    stru_name##Bucket *oldarr = self->arr;                                     \
                                                                               \
    self->size = round_pow2(self->active_cnt * MAP_RESIZE_FACTOR);             \
    self->arr = calloc(self->size, sizeof *self->arr);                         \
                                                                               \
    for (size_t i = 0; i < oldsize; ++i)                                       \
        if (oldarr[i].state == MBS_ACTIVE) {                                   \
            /* Put contents of active bucket into the new table */             \
            size_t perturb = oldarr[i].hash, j = perturb % self->size;         \
            for (;;) {                                                         \
                if (self->arr[j].state != MBS_ACTIVE) {                        \
                    self->arr[j] = oldarr[i];                                  \
                    break;                                                     \
                }                                                              \
                perturb >>= 5;                                                 \
                j = (j * 5 + perturb + 1) % self->size;                        \
            }                                                                  \
        }                                                                      \
                                                                               \
    free(oldarr);                                                              \
}                                                                              \
                                                                               \
static inline vtype *fn_pre##_put(stru_name *self, ktype key)                  \
{                                                                              \
    if (self->active_cnt >= MAP_RESIZE_TRESHOLD(self->size))                   \
        fn_pre##_resize(self);                                                 \
                                                                               \
    size_t hash = khash(key), perturb = hash, i = hash % self->size;           \
                                                                               \
    for (;;) {                                                                 \
        if (self->arr[i].state != MBS_ACTIVE) {                                \
            /* Fill non-active bucket */                                       \
            ++self->active_cnt;                                                \
            break;                                                             \
        }                                                                      \
        if (self->arr[i].hash == hash && !kcmp(self->arr[i].key, key)) {       \
            /* Modify active bucket with a matching key */                     \
            break;                                                             \
        }                                                                      \
        perturb >>= 5;                                                         \
        i = (i * 5 + perturb + 1) % self->size;                                \
    }                                                                          \
                                                                               \
    self->arr[i].state = MBS_ACTIVE;                                           \
    self->arr[i].hash = hash;                                                  \
    self->arr[i].key = key;                                                    \
    return &self->arr[i].val;                                                  \
}                                                                              \
                                                                               \
static inline vtype *fn_pre##_get(stru_name *self, ktype key)                  \
{                                                                              \
    size_t hash = khash(key), perturb = hash, i = hash % self->size;           \
                                                                               \
    while (self->arr[i].state != MBS_EMPTY) {                                  \
        if (self->arr[i].state == MBS_ACTIVE &&                                \
                self->arr[i].hash == hash && !kcmp(self->arr[i].key, key)) {   \
            /* Key of active bucket matches */                                 \
            return &self->arr[i].val;                                          \
        }                                                                      \
        perturb >>= 5;                                                         \
        i = (i * 5 + perturb + 1) % self->size;                                \
    }                                                                          \
    return NULL;                                                               \
}                                                                              \
                                                                               \
static inline void fn_pre##_del(stru_name *self, ktype key)                    \
{                                                                              \
    size_t hash = khash(key), perturb = hash, i = hash % self->size;           \
                                                                               \
    while (self->arr[i].state != MBS_EMPTY) {                                  \
        if (self->arr[i].state == MBS_ACTIVE &&                                \
                self->arr[i].hash == hash && !kcmp(self->arr[i].key, key)) {   \
            /* Key of active bucket matches */                                 \
            self->arr[i].state = MBS_DUMMY;                                    \
            --self->active_cnt;                                                \
            return;                                                            \
        }                                                                      \
        perturb >>= 5;                                                         \
        i = (i * 5 + perturb + 1) % self->size;                                \
    }                                                                          \
}                                                                              \

#endif
