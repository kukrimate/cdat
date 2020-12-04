/*
 * Unordered map tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "map.h"
#include "djb2.h"

MAP_GEN(char *, char *, djb2_hash, !strcmp, s)

#define ihash(x) x
#define icmp(x, y) x == y
MAP_GEN(int, char *, ihash, icmp, i)

struct item {
	char *k, *v;
};

static struct item items[] = {
	{ .k = "some key 0", .v = "some val 0" },
	{ .k = "some key 1", .v = "some val 1" },
	{ .k = "some key 2", .v = "some val 2" },
	{ .k = "some key 3", .v = "some val 3" },
	{ .k = "some key 4", .v = "some val 4" },
	{ .k = "some key 5", .v = "some val 5" },
	{ .k = "some key 6", .v = "some val 6" },
	{ .k = "some key 7", .v = "some val 7" },
	{ .k = "some key 8", .v = "some val 8" },
	{ .k = "some key 9", .v = "some val 9" },
	{ .k = "some key 10", .v = "some val 10" },
	{ .k = "some key 11", .v = "some val 11" },
	{ .k = "some key 12", .v = "some val 12" },
	{ .k = "some key 13", .v = "some val 13" },
	{ .k = "some key 14", .v = "some val 14" },
	{ .k = "some key 15", .v = "some val 15" },
	{ .k = "some key 16", .v = "some val 16" },
	{ .k = "some key 17", .v = "some val 17" },
	{ .k = "some key 18", .v = "some val 18" },
	{ .k = "some key 19", .v = "some val 19" },
	{ .k = "some key 20", .v = "some val 20" },
	{ .k = "some key 21", .v = "some val 21" },
	{ .k = "some key 22", .v = "some val 22" },
	{ .k = "some key 23", .v = "some val 23" },
	{ .k = "some key 24", .v = "some val 24" },
	{ .k = "some key 25", .v = "some val 25" },
	{ .k = "some key 26", .v = "some val 26" },
	{ .k = "some key 27", .v = "some val 27" },
	{ .k = "some key 28", .v = "some val 28" },
	{ .k = "some key 29", .v = "some val 29" },
	{ .k = "some key 30", .v = "some val 30" },
	{ .k = "some key 31", .v = "some val 31" },
	{ .k = "some key 32", .v = "some val 32" },
	{ .k = "some key 33", .v = "some val 33" },
	{ .k = "some key 34", .v = "some val 34" },
	{ .k = "some key 35", .v = "some val 35" },
	{ .k = "some key 36", .v = "some val 36" },
	{ .k = "some key 37", .v = "some val 37" },
	{ .k = "some key 38", .v = "some val 38" },
	{ .k = "some key 39", .v = "some val 39" },
	{ .k = "some key 40", .v = "some val 40" },
	{ .k = "some key 41", .v = "some val 41" },
	{ .k = "some key 42", .v = "some val 42" },
	{ .k = "some key 43", .v = "some val 43" },
	{ .k = "some key 44", .v = "some val 44" },
	{ .k = "some key 45", .v = "some val 45" },
	{ .k = "some key 46", .v = "some val 46" },
	{ .k = "some key 47", .v = "some val 47" },
	{ .k = "some key 48", .v = "some val 48" },
	{ .k = "some key 49", .v = "some val 49" },
	{ .k = "duplicate", .v = "duplicate val" },
	{ .k = "duplicate", .v = "duplicate val" },
};

static void test_string()
{
	size_t i;
	smap h;

	printf("Running string map test... ");

	smap_init(&h);

	for (i = 0; i < sizeof(items) / sizeof(items[0]); ++i)
		smap_put(&h, items[i].k, items[i].v);
	for (i = 0; i < sizeof(items) / sizeof(items[0]); ++i)
		assert(smap_get(&h, items[i].k) == items[i].v);

	smap_free(&h);

	printf("OK\n");
}

struct item1 {
	uint16_t k;
	char *v;
};

static struct item1 items1[] = {
	{ .k = 18, .v = "some val 16" },
	{ .k = 65, .v = "some val 17" },
	{ .k = 6215, .v = "some val 18" },
	{ .k = 15555, .v = "some val 19" },
	{ .k = 145, .v = "some val 20" },
	{ .k = 5676, .v = "some val 21" },
	{ .k = 4356, .v = "some val 22" },
	{ .k = 42456, .v = "some val 23" },
	{ .k = 54244, .v = "some val 24" },
};

static void test_int()
{
	size_t i;
	imap h;

	printf("Running integer map test... ");

	imap_init(&h);

	for (i = 0; i < sizeof(items1) / sizeof(items1[0]); ++i)
		imap_put(&h, items1[i].k, items1[i].v);
	for (i = 0; i < sizeof(items1) / sizeof(items1[0]); ++i)
		assert(items1[i].v == imap_get(&h, items1[i].k));

	imap_free(&h);

	printf("OK\n");
}

int main()
{
	test_string();
	test_int();
}
