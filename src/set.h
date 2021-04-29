/*
 * Unordered set
 */

#ifndef SET_H
#define SET_H

#include <cdat.h>

/* Minimum number of buckets */
#ifndef SET_MINSIZE
#define SET_MINSIZE 8
#endif

/* Maximum number of used buckets allowed for a given table */
#ifndef SET_RESIZE_TRESHOLD
#define SET_RESIZE_TRESHOLD(size) (size * 2 / 3)
#endif

/* The new table size will be round_pow2(self->used * SET_RESIZE_FACTOR) */
#ifndef SET_RESIZE_FACTOR
#define SET_RESIZE_FACTOR 3
#endif


/* State of a bucket */
typedef enum {
    SBS_EMPTY,  // Empty
    SBS_DUMMY,  // Previously active
    SBS_ACTIVE, // Currently active
} SetBucketState;

/*
 * Generate type specific definitions
 */
#define SET_GEN(ktype, khash, kcmp, alias)                                     \
                                                                               \
typedef struct {                                                               \
    SetBucketState state;                                                      \
    size_t hash;                                                               \
    ktype key;                                                                 \
} SetBucket_##alias;                                                           \
                                                                               \
typedef struct {                                                               \
    size_t active_cnt;                                                         \
    size_t size;                                                               \
    SetBucket_##alias *arr;                                                    \
} Set_##alias;                                                                 \
                                                                               \
static inline void set_##alias##_init(Set_##alias *self)                       \
{                                                                              \
    self->active_cnt = 0;                                                      \
    self->size = SET_MINSIZE;                                                  \
    self->arr = calloc(self->size, sizeof *self->arr);                         \
}                                                                              \
                                                                               \
static inline void set_##alias##_free(Set_##alias *self)                       \
{                                                                              \
    free(self->arr);                                                           \
}                                                                              \
                                                                               \
static inline void set_##alias##_resize(Set_##alias *self)                     \
{                                                                              \
    size_t oldsize = self->size;                                               \
    SetBucket_##alias *oldarr = self->arr;                                     \
                                                                               \
    self->size = round_pow2(self->active_cnt * SET_RESIZE_FACTOR);             \
    self->arr = calloc(self->size, sizeof *self->arr);                         \
                                                                               \
    for (size_t i = 0; i < oldsize; ++i)                                       \
        if (oldarr[i].state == SBS_ACTIVE) {                                   \
            /* Put contents of active bucket into the new table */             \
            size_t perturb = oldarr[i].hash, j = perturb % self->size;         \
            for (;;) {                                                         \
                if (self->arr[j].state != SBS_ACTIVE) {                        \
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
static inline void set_##alias##_set(Set_##alias *self, ktype key)             \
{                                                                              \
    if (self->active_cnt >= SET_RESIZE_TRESHOLD(self->size))                   \
        set_##alias##_resize(self);                                            \
                                                                               \
    size_t hash = khash(key), perturb = hash, i = hash % self->size;           \
                                                                               \
    for (;;) {                                                                 \
        if (self->arr[i].state != SBS_ACTIVE) {                                \
            /* Fill non-active bucket */                                       \
            ++self->active_cnt;                                                \
            break;                                                             \
        }                                                                      \
        if (self->arr[i].hash == hash && !kcmp(self->arr[i].key, key)) {       \
            /* Already set, so we can just return */                           \
            return;                                                            \
        }                                                                      \
        perturb >>= 5;                                                         \
        i = (i * 5 + perturb + 1) % self->size;                                \
    }                                                                          \
                                                                               \
    self->arr[i].state = SBS_ACTIVE;                                           \
    self->arr[i].hash = hash;                                                  \
    self->arr[i].key = key;                                                    \
}                                                                              \
                                                                               \
static inline _Bool set_##alias##_isset(Set_##alias *self, ktype key)          \
{                                                                              \
    size_t hash = khash(key), perturb = hash, i = hash % self->size;           \
                                                                               \
    while (self->arr[i].state != SBS_EMPTY) {                                  \
        if (self->arr[i].state == SBS_ACTIVE &&                                \
                self->arr[i].hash == hash && !kcmp(self->arr[i].key, key)) {   \
            /* Key of active bucket matches */                                 \
            return 1;                                                          \
        }                                                                      \
        perturb >>= 5;                                                         \
        i = (i * 5 + perturb + 1) % self->size;                                \
    }                                                                          \
    return 0;                                                                  \
}                                                                              \
                                                                               \
static inline void set_##alias##_unset(Set_##alias *self, ktype key)           \
{                                                                              \
    size_t hash = khash(key), perturb = hash, i = hash % self->size;           \
                                                                               \
    while (self->arr[i].state != SBS_EMPTY) {                                  \
        if (self->arr[i].state == SBS_ACTIVE &&                                \
                self->arr[i].hash == hash && !kcmp(self->arr[i].key, key)) {   \
            /* Key of active bucket matches */                                 \
            self->arr[i].state = SBS_DUMMY;                                    \
            --self->active_cnt;                                                \
            return;                                                            \
        }                                                                      \
        perturb >>= 5;                                                         \
        i = (i * 5 + perturb + 1) % self->size;                                \
    }                                                                          \
}                                                                              \

#endif
