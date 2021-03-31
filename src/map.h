/*
 * Unordered map
 */

#ifndef MAP_H
#define MAP_H

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

/* Round to the nearest power of 2 >= to size */
static inline size_t round_pow2(size_t size)
{
    size_t newsize = MAP_MINSIZE;
    while (newsize < size)
        newsize <<= 1;
    return newsize;
}

/* State of a bucket */
typedef enum {
    BS_EMPTY,  // Empty
    BS_DUMMY,  // Previously active
    BS_ACTIVE, // Currently active
} BucketState;

/*
 * Generate type specific definitions
 */
#define MAP_GEN(ktype, vtype, khash, kcmp, alias)                              \
                                                                               \
typedef struct {                                                               \
    BucketState state;                                                         \
    size_t hash;                                                               \
    ktype key;                                                                 \
    vtype val;                                                                 \
} Bucket_##alias;                                                              \
                                                                               \
typedef struct {                                                               \
    size_t active_cnt;                                                         \
    size_t size;                                                               \
    Bucket_##alias *arr;                                                       \
} Map_##alias;                                                                 \
                                                                               \
static inline void map_##alias##_init(Map_##alias *self)                       \
{                                                                              \
    self->active_cnt = 0;                                                      \
    self->size = MAP_MINSIZE;                                                  \
    self->arr = calloc(self->size, sizeof *self->arr);                         \
}                                                                              \
                                                                               \
static inline void map_##alias##_free(Map_##alias *self)                       \
{                                                                              \
    free(self->arr);                                                           \
}                                                                              \
                                                                               \
static inline void map_##alias##_resize(Map_##alias *self)                     \
{                                                                              \
    size_t oldsize = self->size;                                               \
    Bucket_##alias *oldarr = self->arr;                                        \
                                                                               \
    self->size = round_pow2(self->active_cnt * MAP_RESIZE_FACTOR);             \
    self->arr = calloc(self->size, sizeof *self->arr);                         \
                                                                               \
    for (size_t i = 0; i < oldsize; ++i)                                       \
        if (oldarr[i].state == BS_ACTIVE) {                                    \
            /* Put contents of active bucket into the new table */             \
            size_t perturb = oldarr[i].hash, j = perturb % self->size;         \
            for (;;) {                                                         \
                if (self->arr[j].state != BS_ACTIVE) {                         \
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
static inline vtype *map_##alias##_put(Map_##alias *self, ktype key)           \
{                                                                              \
    if (self->active_cnt >= MAP_RESIZE_TRESHOLD(self->size))                   \
        map_##alias##_resize(self);                                            \
                                                                               \
    size_t hash = khash(key), perturb = hash, i = hash % self->size;           \
                                                                               \
    for (;;) {                                                                 \
        if (self->arr[i].state != BS_ACTIVE) {                                 \
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
    self->arr[i].state = BS_ACTIVE;                                            \
    self->arr[i].hash = hash;                                                  \
    self->arr[i].key = key;                                                    \
    return &self->arr[i].val;                                                  \
}                                                                              \
                                                                               \
static inline vtype *map_##alias##_get(Map_##alias *self, ktype key)           \
{                                                                              \
    size_t hash = khash(key), perturb = hash, i = hash % self->size;           \
                                                                               \
    while (self->arr[i].state != BS_EMPTY) {                                   \
        if (self->arr[i].state == BS_ACTIVE &&                                 \
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
static inline void map_##alias##_del(Map_##alias *self, ktype key)             \
{                                                                              \
    size_t hash = khash(key), perturb = hash, i = hash % self->size;           \
                                                                               \
    while (self->arr[i].state != BS_EMPTY) {                                   \
        if (self->arr[i].state == BS_ACTIVE &&                                 \
                self->arr[i].hash == hash && !kcmp(self->arr[i].key, key)) {   \
            /* Key of active bucket matches */                                 \
            self->arr[i].state = BS_DUMMY;                                     \
            --self->active_cnt;                                                \
            return;                                                            \
        }                                                                      \
        perturb >>= 5;                                                         \
        i = (i * 5 + perturb + 1) % self->size;                                \
    }                                                                          \
}                                                                              \

#endif
