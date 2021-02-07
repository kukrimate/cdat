/*
 * Priority queue
 */

#ifndef PQ_H
#define PQ_H

/*
 * Starting size of the underlying array
 */
#ifndef PQ_PREALLOC
#define PQ_PREALLOC 10
#endif

#define PQ_GEN(type, cmp, alias) \
typedef struct { \
    /* Number of nodes in the tree */ \
    size_t n; \
    /* Current size of the array */ \
    size_t array_size; \
    /* Underlying array, storing tree nodes row-by-row, left-right */ \
    type *array; \
} PQ##alias; \
 \
static inline void PQ##alias##_init(PQ##alias *self) \
{ \
    self->n = 0; \
    self->array_size = PQ_PREALLOC; \
    self->array = reallocarray(NULL, self->array_size, sizeof *self->array); \
} \
 \
static inline void PQ##alias##_free(PQ##alias *self) \
{ \
    free(self->array); \
} \
 \
static inline void PQ##alias##_push(PQ##alias *self, int new_val) \
{ \
    size_t new_idx, parent_idx; \
    type tmp_val; \
 \
    /* Double capacity if the array is full */ \
    if (self->n >= self->array_size) { \
        self->array_size = self->n << 1; \
        self->array = reallocarray(self->array, self->array_size, sizeof *self->array); \
    } \
 \
    /* Add new node at the end */ \
    new_idx = self->n++; \
    self->array[new_idx] = new_val; \
    /* Move new node down */ \
    while (new_idx && cmp(self->array[new_idx], self->array[parent_idx = (new_idx - 1) >> 1])) { \
        /* Do a swap */ \
        tmp_val = self->array[new_idx]; \
        self->array[new_idx] = self->array[parent_idx]; \
        self->array[parent_idx] = tmp_val; \
        /* Parent becomes the new current index */ \
        new_idx = parent_idx; \
    } \
} \
 \
static inline void PQ##alias##heapify(type *array, size_t size, size_t root) \
{ \
    size_t min_idx, left_idx, right_idx; \
    type tmp_val; \
 \
    for (;;) { \
        min_idx = root; \
        left_idx = root << 1 | 1; \
        right_idx = left_idx + 1; \
 \
        /* Check left child */ \
        if (left_idx < size && cmp(array[left_idx], array[min_idx])) \
            min_idx = left_idx; \
        /* Check right child */ \
        if (right_idx < size && cmp(array[right_idx], array[min_idx])) \
            min_idx = right_idx; \
 \
        /* If none of the children are smaller, we are done */ \
        if (min_idx == root) \
            return; \
 \
        /* Smallest children becomes the root */ \
        tmp_val = array[root]; \
        array[root] = array[min_idx]; \
        array[min_idx] = tmp_val; \
 \
        /* "Recurse" with the new root */ \
        root = min_idx; \
    } \
} \
 \
static inline type PQ##alias##_pop(PQ##alias *self) \
{ \
    type root_val; \
 \
    /* Save root value */ \
    root_val = self->array[0]; \
 \
    /* Move end node to the root */ \
    self->array[0] = self->array[--self->n]; \
 \
    /* Make sure the new tree is still a min-heap */ \
    PQ##alias##heapify(self->array, self->n, 0); \
    return root_val; \
}

#endif
