/*
 * Tests for dynamic array
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dynarr.h"

dynarr_gen(char, char)
dynarr_gen(void *, pvoid)

struct my {
	short a, b;
	int   c, d;
};

dynarr_gen(struct my, smy)

static struct my teststructs[] = {
	{ .a = 0, .b = 0, .c = 0, .d = 0 },
	{ .a = 99, .b = 70, .c = 89, .d = 11 },
	{ .a = 33, .b = 78, .c = 14, .d = 62 },
	{ .a = 40, .b = 65, .c = 56, .d = 16 },
	{ .a = 76, .b = 61, .c = 21, .d = 82 },
	{ .a = 75, .b = 62, .c = 25, .d = 33 },
	{ .a = 8, .b = 78, .c = 18, .d = 67 },
	{ .a = 83, .b = 26, .c = 30, .d = 95 },
	{ .a = 85, .b = 63, .c = 32, .d = 29 },
	{ .a = 18, .b = 28, .c = 1, .d = 24 },
	{ .a = 37, .b = 85, .c = 88, .d = 13 },
	{ .a = 85, .b = 3, .c = 70, .d = 50 },
	{ .a = 0, .b = 0, .c = 0, .d = 0 },
	{ .a = 19, .b = 65, .c = 39, .d = 31 },
	{ .a = 75, .b = 28, .c = 6, .d = 93 },
	{ .a = 1, .b = 82, .c = 87, .d = 50 },
	{ .a = 63, .b = 72, .c = 40, .d = 96 },
	{ .a = 32, .b = 6, .c = 44, .d = 22 },
	{ .a = 60, .b = 0, .c = 50, .d = 80 },
	{ .a = 72, .b = 28, .c = 6, .d = 80 },
	{ .a = 31, .b = 44, .c = 62, .d = 6 },
	{ .a = 23, .b = 15, .c = 53, .d = 8 },
	{ .a = 69, .b = 14, .c = 6, .d = 2 },
	{ .a = 100, .b = 55, .c = 97, .d = 1 },
	{ .a = 60, .b = 73, .c = 48, .d = 88 },
	{ .a = 75, .b = 45, .c = 34, .d = 46 },
	{ .a = 64, .b = 32, .c = 57, .d = 18 },
	{ .a = 59, .b = 29, .c = 99, .d = 16 },
	{ .a = 37, .b = 52, .c = 96, .d = 59 },
	{ .a = 83, .b = 22, .c = 29, .d = 92 },
	{ .a = 54, .b = 47, .c = 49, .d = 51 },
	{ .a = 90, .b = 24, .c = 75, .d = 84 },
	{ .a = 26, .b = 54, .c = 55, .d = 71 },
	{ .a = 47, .b = 5, .c = 66, .d = 56 },
	{ .a = 65, .b = 54, .c = 95, .d = 88 },
	{ .a = 62, .b = 38, .c = 52, .d = 80 },
	{ .a = 42, .b = 81, .c = 93, .d = 32 },
	{ .a = 26, .b = 47, .c = 39, .d = 1 },
	{ .a = 59, .b = 22, .c = 67, .d = 100 },
	{ .a = 90, .b = 66, .c = 59, .d = 23 },
	{ .a = 36, .b = 68, .c = 22, .d = 89 },
	{ .a = 45, .b = 69, .c = 35, .d = 62 },
	{ .a = 5, .b = 64, .c = 33, .d = 100 },
	{ .a = 84, .b = 64, .c = 17, .d = 78 },
	{ .a = 64, .b = 49, .c = 45, .d = 3 },
	{ .a = 8, .b = 5, .c = 92, .d = 79 },
	{ .a = 86, .b = 72, .c = 25, .d = 32 },
	{ .a = 100, .b = 23, .c = 66, .d = 32 },
	{ .a = 38, .b = 61, .c = 49, .d = 82 },
	{ .a = 63, .b = 65, .c = 32, .d = 45 },
	{ .a = 50, .b = 10, .c = 22, .d = 47 },
	{ .a = 47, .b = 35, .c = 43, .d = 23 },
	{ .a = 72, .b = 95, .c = 15, .d = 34 },
	{ .a = 77, .b = 95, .c = 72, .d = 30 },
	{ .a = 44, .b = 75, .c = 88, .d = 9 },
	{ .a = 26, .b = 2, .c = 63, .d = 75 },
	{ .a = 69, .b = 84, .c = 21, .d = 20 },
	{ .a = 16, .b = 25, .c = 7, .d = 31 },
	{ .a = 23, .b = 28, .c = 9, .d = 85 },
	{ .a = 41, .b = 53, .c = 4, .d = 98 },
	{ .a = 21, .b = 22, .c = 99, .d = 37 },
	{ .a = 37, .b = 11, .c = 62, .d = 0 },
	{ .a = 49, .b = 4, .c = 59, .d = 31 },
	{ .a = 26, .b = 44, .c = 91, .d = 38 },
	{ .a = 77, .b = 92, .c = 3, .d = 39 },
	{ .a = 2, .b = 85, .c = 4, .d = 95 },
	{ .a = 37, .b = 13, .c = 10, .d = 16 },
	{ .a = 65, .b = 43, .c = 72, .d = 71 },
	{ .a = 74, .b = 8, .c = 31, .d = 77 },
	{ .a = 9, .b = 49, .c = 88, .d = 30 },
	{ .a = 69, .b = 75, .c = 8, .d = 27 },
	{ .a = 65, .b = 36, .c = 20, .d = 79 },
	{ .a = 56, .b = 73, .c = 52, .d = 15 },
	{ .a = 90, .b = 23, .c = 18, .d = 90 },
	{ .a = 47, .b = 33, .c = 99, .d = 4 },
	{ .a = 80, .b = 39, .c = 94, .d = 42 },
	{ .a = 50, .b = 2, .c = 70, .d = 30 },
	{ .a = 76, .b = 7, .c = 69, .d = 16 },
	{ .a = 66, .b = 53, .c = 31, .d = 83 },
	{ .a = 5, .b = 88, .c = 53, .d = 61 },
	{ .a = 60, .b = 91, .c = 40, .d = 46 },
	{ .a = 91, .b = 23, .c = 7, .d = 3 },
	{ .a = 47, .b = 88, .c = 72, .d = 80 },
	{ .a = 14, .b = 85, .c = 57, .d = 98 },
	{ .a = 85, .b = 79, .c = 49, .d = 44 },
	{ .a = 36, .b = 22, .c = 55, .d = 55 },
	{ .a = 26, .b = 15, .c = 87, .d = 41 },
	{ .a = 59, .b = 40, .c = 44, .d = 14 },
	{ .a = 56, .b = 27, .c = 71, .d = 17 },
	{ .a = 5, .b = 75, .c = 8, .d = 69 },
	{ .a = 95, .b = 46, .c = 86, .d = 9 },
	{ .a = 26, .b = 19, .c = 54, .d = 19 },
	{ .a = 93, .b = 26, .c = 44, .d = 44 },
	{ .a = 69, .b = 29, .c = 95, .d = 43 },
	{ .a = 84, .b = 24, .c = 31, .d = 26 },
	{ .a = 15, .b = 26, .c = 66, .d = 41 },
	{ .a = 7, .b = 55, .c = 3, .d = 57 },
	{ .a = 15, .b = 32, .c = 40, .d = 62 },
	{ .a = 1, .b = 39, .c = 15, .d = 23 },
	{ .a = 73, .b = 24, .c = 76, .d = 53 },
	{ .a = 85, .b = 10, .c = 32, .d = 51 },
};

static void test_arr()
{
	struct dynarr(smy) a;
	size_t i;

	printf("Running generic array test... ");

	dynarr_alloc(smy)(&a);

	for (i = 0; i < sizeof(teststructs) / sizeof(struct my); ++i)
		dynarr_add(smy)(&a, teststructs[i]);
	assert(!memcmp(a.array, teststructs, sizeof(teststructs)));

	dynarr_free(smy)(&a);
	printf("OK\n");
}

static char *strings[] = {
	"Test string 1",
	"String2\n\raggagagkakjga",
	"{{$$strdata$$}}",
};
static char *concated = "Test string 1String2\n\raggagagkakjga{{$$strdata$$}}";

static void test_chararr()
{
	struct dynarr(char) a;
	size_t i, j;

	printf("Running character array test... ");

	dynarr_alloc(char)(&a);
	for (i = 0; i < sizeof(strings) / sizeof(char *); ++i)
		for (j = 0; j < strlen(strings[i]); ++j)
			dynarr_add(char)(&a, strings[i][j]);
	assert(!strncmp(concated, a.array, a.nmemb));

	dynarr_free(char)(&a);

	printf("OK\n");
}

static void *ptrs[] = {
	NULL,
	(void *) 0xDEADBEEF,
	teststructs,
	strings
};

static void test_ptrarr()
{
	struct dynarr(pvoid) a;
	size_t i;

	printf("Running pointer array test... ");

	dynarr_alloc(pvoid)(&a);
	for (i = 0; i < sizeof(ptrs) / sizeof(void *); ++i)
		dynarr_add(pvoid)(&a, ptrs[i]);
	assert(!memcmp(ptrs, a.array, sizeof(ptrs)));

	dynarr_free(pvoid)(&a);

	printf("OK\n");
}

int main()
{
	test_arr();
	test_chararr();
	test_ptrarr();
}
