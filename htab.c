/*
 * Hash table using DJB2 and chaining
 * Auhor: Mate Kukri
 * License: ISC
 */
#include <stdlib.h>
#include <string.h>
#include "htab.h"

#define HTAB_GROW_FACTOR   2
#define HTAB_START_BUCKETS 16
#define HTAB_LOAD_LIMIT(x) (x * 3 / 4)

static size_t djb2_hash(char *str)
{
	size_t r;

	r = 5381;
	for (; *str; ++str)
		r = (r << 15) + r + *str;
	return r;
}

/*
 * Put key/value pairs into a hashtable *without* checking for key overwrites
 */
static void htab_putuniq(struct htab *self, char *key, char *val)
{
	struct helem **ptr;

	/* Find start of target bucket */
	ptr = self->buckets + djb2_hash(key) % self->bucket_cnt;

	if (!*ptr)
		/* New bucket */
		++self->filled_cnt;
	else
		/* Previously used bucket */
		for (; *ptr; ptr = &(*ptr)->next);

	/* Store key/value pair */
	*ptr = malloc(sizeof(struct helem));
	if (!*ptr)
		abort();
	(*ptr)->key = key;
	(*ptr)->value = val;
	(*ptr)->next = NULL;
}

static void htab_grow(struct htab *self)
{
	struct helem **old, **cur, **end, *node, *tmp;

	/* Save old bucket array pointer for freeing */
	old = self->buckets;

	/* Pointers for iterating through the old bucket array */
	cur = old;
	end = old + self->bucket_cnt;

	/* Allocate new bucket array */
	self->filled_cnt  = 0;
	self->bucket_cnt *= HTAB_GROW_FACTOR;
	self->buckets     = calloc(self->bucket_cnt, sizeof(struct helem *));
	if (!self->buckets)
		abort();

	/* Put the contents of the old bucket array into the new */
	for (; cur < end; ++cur)
		for (node = *cur; node; tmp = node->next, free(node), node = tmp)
			htab_putuniq(self, node->key, node->value);

	/* Free old bucket array */
	free(old);
}

void htab_alloc(struct htab *self)
{
	self->filled_cnt = 0;
	self->bucket_cnt = HTAB_START_BUCKETS;
	self->buckets    = calloc(self->bucket_cnt, sizeof(struct helem *));
	if (!self->buckets)
		abort();
}

void htab_free(struct htab *self, int d)
{
	struct helem **cur, **end, *node, *tmp;

	cur = self->buckets;
	end = cur + self->bucket_cnt;

	for (; cur < end; ++cur)
		for (node = *cur; node; tmp = node->next, free(node), node = tmp)
			if (d) {
				free(node->key);
				free(node->value);
			}

	free(self->buckets);
}

void htab_put(struct htab *self, char *key, char *val, int d)
{
	struct helem **ptr;

	/* Grow the hashtable if necessary */
	if (self->filled_cnt > HTAB_LOAD_LIMIT(self->bucket_cnt))
		htab_grow(self);

	ptr = self->buckets + djb2_hash(key) % self->bucket_cnt;

	if (!*ptr)
		++self->filled_cnt;
	else
		for (; *ptr; ptr = &(*ptr)->next)
			if (!strcmp((*ptr)->key, key)) {
				if (d) {
					free((*ptr)->key);
					free((*ptr)->value);
				}
				goto insert;
			}

	*ptr = malloc(sizeof(struct helem));
	if (!*ptr)
		abort();
	(*ptr)->next = NULL;
insert:
	(*ptr)->key = key;
	(*ptr)->value = val;
}

char *htab_get(struct htab *self, char *key)
{
	struct helem **ptr;

	ptr = self->buckets + djb2_hash(key) % self->bucket_cnt;

	for (; *ptr; ptr = &(*ptr)->next)
		if (!strcmp((*ptr)->key, key))
			return (*ptr)->value;

	return NULL;
}
