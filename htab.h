#ifndef HTAB_H
#define HTAB_H

typedef struct {
	/* # of filled buckets */
	size_t filled_cnt;
	/* # of buckets */
	size_t bucket_cnt;
	/* Buffer */
	void  *buffer;
} htab;

/*
 * Create a new hashtable with n preallocated buckets
 */
void htab_alloc(htab *x);

/*
 * Delete a hashtable freeing all memory used, if d is set
 * than key/values will also be free'd
 */
void htab_free(htab *x, int d);

/*
 * Insert a key/value pair into a hashtable, if d is set
 * than key/values will be free'd before overwriting
 */
void htab_put(htab *x, char *key, char *val, int d);

/*
 * Retrieve a value mapped to a given key,
 * if the key is not in the table NULL is returned
 */
char *htab_get(htab *x, char *key);

#endif
