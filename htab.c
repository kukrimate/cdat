/*
 * Hash table using DJB2 and chaining
 * Auhor: Mate Kukri
 * License: ISC
 */
#include <stdlib.h>
#include <string.h>
#include "htab.h"

typedef struct helem helem;
struct helem {
	char  *key;
	char  *val;
	helem *nex;
};

#define HTAB_GROW_FACTOR   2
#define HTAB_START_BUCKETS 16
#define HTAB_LOAD_LIMIT(x) (x * 3 / 4)

static size_t djb2_hash(char *str)
{
	size_t r;

	r = 5381;
	for (; *str; ++str)
		r = (r << 5) + r + *str;
	return r;
}

/*
 * Put key/value paris into a hashtable *without* checking for key overwrites
 */
static void htab_putuniq(htab *x, char *key, char *val)
{
	helem **ptr;

	ptr  = x->buffer;
	ptr += djb2_hash(key) % x->bucket_cnt;;

	if (!*ptr)
		++x->filled_cnt;
	else
		for (; *ptr; ptr = &(*ptr)->nex);

	*ptr = malloc(sizeof(helem));
	if (!*ptr)
		abort();
	(*ptr)->key = key;
	(*ptr)->val = val;
	(*ptr)->nex = NULL;
}

static void htab_grow(htab *x)
{
	helem **start, **cur, **end, *node, *tmp;

	start = cur = x->buffer;
	end   = cur + x->bucket_cnt;

	x->filled_cnt  = 0;
	x->bucket_cnt *= HTAB_GROW_FACTOR;
	x->buffer      = calloc(x->bucket_cnt, sizeof(helem *));
	if (!x->buffer)
		abort();

	for (; cur < end; ++cur)
		for (node = *cur; node; tmp = node->nex, free(node), node = tmp)
			htab_putuniq(x, node->key, node->val);

	free(start);
}

void htab_alloc(htab *x)
{
	x->filled_cnt = 0;
	x->bucket_cnt = HTAB_START_BUCKETS;
	x->buffer     = calloc(x->bucket_cnt, sizeof(helem *));
	if (!x->buffer)
		abort();
}

void htab_free(htab *x, int d)
{
	helem **bucket, **end, *node, *tmp;

	bucket = x->buffer;
	end    = bucket + x->bucket_cnt;

	for (; bucket < end; ++bucket)
		for (node = *bucket; node; tmp = node->nex, free(node), node = tmp)
			if (d) {
				free(node->key);
				free(node->val);
			}

	free(x->buffer);
}

void htab_put(htab *x, char *key, char *val, int d)
{
	helem **ptr;

	if (x->filled_cnt > HTAB_LOAD_LIMIT(x->bucket_cnt))
		htab_grow(x);

	ptr  = x->buffer;
	ptr += djb2_hash(key) % x->bucket_cnt;

	if (!*ptr)
		++x->filled_cnt;
	else
		for (; *ptr; ptr = &(*ptr)->nex)
			if (!strcmp((*ptr)->key, key)) {
				if (d) {
					free((*ptr)->key);
					free((*ptr)->val);
				}
				goto insert;
			}

	*ptr = malloc(sizeof(helem));
	if (!*ptr)
		abort();
	(*ptr)->nex = NULL;
insert:
	(*ptr)->key = key;
	(*ptr)->val = val;
}

char *htab_get(htab *x, char *key)
{
	helem **ptr;

	ptr  = x->buffer;
	ptr += djb2_hash(key) % x->bucket_cnt;

	for (; *ptr; ptr = &(*ptr)->nex)
		if (!strcmp((*ptr)->key, key))
			return (*ptr)->val;

	return NULL;
}
