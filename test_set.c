/*
 * Unordered set tests
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "set.h"
#include "djb2.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

set_gen(const char *, djb2_hash, !strcmp, s)

/* List of string to add to the set */
static const char *str_set[] = {
	"XypOofUdRxRcCBQKZCQlEyxPU"
	"gUNnSLlgEQyUiYHUHjpJgUumU"
	"nJAcZSTiWyuCkdKfMfSfeLCpY"
	"yVxjeUvybfNwsUowvsDmuVvHB"
	"SnLSmHGrOknVOXvAWDpeGukVe"
	"idrTWjuItFeVHBmAQNhNYZEYY"
	"qXKSexXMQzuarYeCsYJyHzAXe"
	"EdHkoHeUIIqrXzLojbaQPjhXr"
	"tkKufRlvrnRXBYjCeKbQYVmDy"
	"kewgcXsvcpjVZnUhGRIqgrXSR"
	"kaJPaedhodvYspLQplJvbzGPf"
	"kzPtCTMWafRCFYLjFDUbWrWgD"
	"ZjGYnxbTHQoTYCzlsHowVCuSh"
	"tCKYZvgVzewLNIqHUJtaZShzg"
	"FiYrvEhPCIHsSpKUiTiapXXPP"
	"oqSvIbdtGrPGmUFUiTdyTWSNW"
	"ZRSiRHdhAyxHAwntIsRDJnRPJ"
	"MxkxtzuKsTLLqrLZFeYrcIPIV"
	"OmOisiLrgagkpHExeGvUBHrcZ"
	"PIkkIndLNqpxFjxquVilMMhuv"
};

/* List of string to test for invalid strings with */
static const char *str_notset[] = {
	"hmFzmZKaxCizALA"
	"tWWoceEzznOFFoU"
	"RiPYMKWctTxSONp"
	"WwsIuvuasFQJOVr"
	"ZEMnaiKFmPXGzLv"
	"hZcDMikkCAjqNER"
	"oGhUeEjHcDTWFzX"
	"HfDQpbENCGoFxpt"
};

static void
test_str(void)
{
	struct sset s;
	size_t i;

	printf("Running string set test... ");
	sset_init(&s);
	for (i = 0; i < ARRAY_SIZE(str_set); ++i)
		sset_set(&s, str_set[i]);
	for (i = 0; i < ARRAY_SIZE(str_notset); ++i)
		assert(!sset_isset(&s, str_notset[i]));
	for (i = 0; i < ARRAY_SIZE(str_set); ++i)
		assert(sset_isset(&s, str_set[i]));
	sset_free(&s);
	printf("OK\n");
}

#define ihash(x) x
#define icmp(x, y) x == y
set_gen(int, ihash, icmp, i)

static void
test_int(void)
{
	struct iset s;
	size_t i, j;
	int int_set[1000], tmp;

	printf("Running integer set test... ");
	iset_init(&s);
	srand(time(NULL)); /* Make sure we get different ints each time */
	for (i = 0; i < ARRAY_SIZE(int_set); ++i) {
		int_set[i] = rand();
		iset_set(&s, int_set[i]);
	}
	for (i = 0; i < ARRAY_SIZE(int_set) / 2; ++i) {
retry:
		tmp = rand();
		for (j = 0; j < ARRAY_SIZE(int_set); ++j)
			if (int_set[j] == tmp)
				goto retry;
		assert(!iset_isset(&s, tmp));
	}
	for (i = 0; i < ARRAY_SIZE(int_set); ++i)
		assert(iset_isset(&s, int_set[i]));
	iset_free(&s);
	printf("OK\n");
}

int
main(void)
{
	test_str();
	test_int();
}
