/*
 * Chained hash table
 * Copyright (C) Mate Kurki, 2020
 * Part of libkm, released under the ISC license
 */
#ifndef HTAB_H
#define HTAB_H

/*
 * Number of buckets to start with
 */
#define HTAB_START_BUCKETS 16

/*
 * Calculate the max allowed number of filled buckets
 */
#define HTAB_LOAD_LIMIT(n) (n * 3 / 4)

/*
 * Factor to grow the bucket count by when the load limit is reached
 */
#define HTAB_GROW_FACTOR 2

/*
 * Generate type specific definitions
 */
#define htab_gen(T, U, hash, cmp, A) \
\
struct helem##A { \
	T key; \
	U value; \
	struct helem##A *next; \
}; \
\
struct htab##A { \
	size_t filled_cnt; \
	size_t bucket_cnt; \
	struct helem##A **buckets; \
}; \
\
static inline void htab_alloc##A(struct htab##A *self) \
{ \
	self->filled_cnt = 0; \
	self->bucket_cnt = HTAB_START_BUCKETS; \
	self->buckets = calloc(self->bucket_cnt, sizeof(struct helem##A *)); \
} \
\
static inline void htab_free##A(struct htab##A *self) \
{ \
	struct helem##A **cur, *node, *tmp; \
\
	for (cur = self->buckets; cur < self->buckets + self->bucket_cnt; ++cur) \
		for (node = *cur; node; tmp = node->next, free(node), node = tmp); \
\
	free(self->buckets); \
} \
static inline void htab_putuniq##A(struct htab##A *self, T key, U val) \
{ \
	struct helem##A **ptr; \
\
	ptr = self->buckets + hash(key) % self->bucket_cnt; \
\
	if (!*ptr) \
		++self->filled_cnt; \
	else \
		for (; *ptr; ptr = &(*ptr)->next); \
\
	*ptr = malloc(sizeof(struct helem##A)); \
	(*ptr)->key = key; \
	(*ptr)->value = val; \
	(*ptr)->next = NULL; \
} \
\
static inline void htab_grow##A(struct htab##A *self) \
{ \
	struct helem##A **old, **cur, **end, *node, *tmp; \
\
	cur = old = self->buckets; \
	end = cur + self->bucket_cnt; \
\
	self->filled_cnt = 0; \
	self->bucket_cnt *= HTAB_GROW_FACTOR; \
	self->buckets = calloc(self->bucket_cnt, sizeof(struct helem *)); \
\
	for (; cur < end; ++cur) \
		for (node = *cur; node; tmp = node->next, free(node), node = tmp) \
			htab_putuniq##A(self, node->key, node->value); \
\
	free(old); \
} \
\
static inline void htab_put##A(struct htab##A *self, T key, U val) \
{ \
	struct helem##A **ptr; \
\
	if (self->filled_cnt > HTAB_LOAD_LIMIT(self->bucket_cnt)) \
		htab_grow##A(self); \
\
	ptr = self->buckets + hash(key) % self->bucket_cnt; \
\
	if (!*ptr) \
		++self->filled_cnt; \
	else \
		for (; *ptr; ptr = &(*ptr)->next) \
			if (cmp((*ptr)->key, key)) \
				goto insert; \
\
	*ptr = malloc(sizeof(struct helem##A)); \
	(*ptr)->next = NULL; \
insert: \
	(*ptr)->key = key; \
	(*ptr)->value = val; \
} \
\
static inline U htab_get##A(struct htab##A *self, T key) \
{ \
	struct helem##A **ptr; \
\
	ptr = self->buckets + hash(key) % self->bucket_cnt; \
\
	for (; *ptr; ptr = &(*ptr)->next) \
		if (cmp((*ptr)->key, key)) \
			return (*ptr)->value; \
\
	return NULL; \
}

/*
 * Macros for nicer syntax
 */
#define htab(x)       htab##x
#define htab_alloc(x) htab_alloc##x
#define htab_free(x)  htab_free##x
#define htab_put(x)   htab_put##x
#define htab_get(x)   htab_get##x

/*
 * Pre-defined hash and compare functions for strings
 */
static inline size_t str_hash(char *str)
{
	size_t r;

	r = 5381;
	for (; *str; ++str)
		r = (r << 15) + r + *str;
	return r;
}

static inline int str_cmp(char *str1, char *str2)
{
	return !strcmp(str1, str2);
}


#endif
