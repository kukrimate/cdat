#ifndef HTAB_H
#define HTAB_H

struct helem {
	char *key, *value;
	struct helem *next;
};

struct htab {
	/* # of filled buckets */
	size_t filled_cnt;
	/* # of buckets */
	size_t bucket_cnt;
	/* Bucker array */
	struct helem **buckets;
};

/*
 * Create a new hashtable
 */
void htab_alloc(struct htab *self);

/*
 * Delete a hashtable freeing all memory used, if d is set
 * than key/values will also be free'd
 */
void htab_free(struct htab *self, int d);

/*
 * Insert a key/value pair into a hashtable, if d is set
 * than key/values will be free'd before overwriting
 */
void htab_put(struct htab *self, char *key, char *val, int d);

/*
 * Retrieve a value mapped to a given key,
 * if the key is not in the table NULL is returned
 */
char *htab_get(struct htab *self, char *key);

#endif
