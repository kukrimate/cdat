/*
 * Unordered set tests
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <set.h>
#include <djb2.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

SET_GEN(const char *, djb2_hash, !strcmp, str)

/* List of string to add to the set */
static const char *str_set[] = {
	"XypOofUdRxRcCBQKZCQlEyxPU",
	"gUNnSLlgEQyUiYHUHjpJgUumU",
	"nJAcZSTiWyuCkdKfMfSfeLCpY",
	"yVxjeUvybfNwsUowvsDmuVvHB",
	"SnLSmHGrOknVOXvAWDpeGukVe",
	"idrTWjuItFeVHBmAQNhNYZEYY",
	"qXKSexXMQzuarYeCsYJyHzAXe",
	"EdHkoHeUIIqrXzLojbaQPjhXr",
	"tkKufRlvrnRXBYjCeKbQYVmDy",
	"kewgcXsvcpjVZnUhGRIqgrXSR",
	"kaJPaedhodvYspLQplJvbzGPf",
	"kzPtCTMWafRCFYLjFDUbWrWgD",
	"ZjGYnxbTHQoTYCzlsHowVCuSh",
	"tCKYZvgVzewLNIqHUJtaZShzg",
	"FiYrvEhPCIHsSpKUiTiapXXPP",
	"oqSvIbdtGrPGmUFUiTdyTWSNW",
	"ZRSiRHdhAyxHAwntIsRDJnRPJ",
	"MxkxtzuKsTLLqrLZFeYrcIPIV",
	"OmOisiLrgagkpHExeGvUBHrcZ",
	"PIkkIndLNqpxFjxquVilMMhuv",
};

/* List of string to test for invalid strings with */
static const char *str_notset[] = {
	"hmFzmZKaxCizALA",
	"tWWoceEzznOFFoU",
	"RiPYMKWctTxSONp",
	"WwsIuvuasFQJOVr",
	"ZEMnaiKFmPXGzLv",
	"hZcDMikkCAjqNER",
	"oGhUeEjHcDTWFzX",
	"HfDQpbENCGoFxpt",
	"sFkPNcDnmZk2EP5",
	"K8MUVt2cvfpZ7Cv",
	"Be41gHuYOd6mFrJ",
	"xjelYxKGMZRYIz8",
	"UhwruA4TwxiegxD",
	"TTFKu2mseBEuwBa",
	"g8lwNqiOYTHj9Se",
	"3GQ3ph8fBzUziUq",
	"BOq3kMj3RhLXGpz",
	"xUWm2yF1tmdlORO",
};

static void
test_str(void)
{
	SETstr s;
	size_t i;

	printf("Running string set test... ");
	SETstr_init(&s);
	for (i = 0; i < ARRAY_SIZE(str_set); ++i)
		SETstr_set(&s, str_set[i]);
	for (i = 0; i < ARRAY_SIZE(str_notset); ++i)
		assert(!SETstr_isset(&s, str_notset[i]));
	for (i = 0; i < ARRAY_SIZE(str_set); ++i)
		assert(SETstr_isset(&s, str_set[i]));

	for (i = 0; i < ARRAY_SIZE(str_set); ++i)
		SETstr_unset(&s, str_set[i]);
	for (i = 0; i < ARRAY_SIZE(str_notset); ++i)
		SETstr_set(&s, str_notset[i]);

	SETstr_free(&s);
	printf("OK\n");
}

#define ihash(x) x
#define icmp(x, y) x == y
SET_GEN(int, ihash, icmp, int)

static void
test_int(void)
{
	SETint s;
	size_t i, j;
	int int_set[1000], tmp;

	printf("Running integer set test... ");
	SETint_init(&s);
	srand(time(NULL)); /* Make sure we get different ints each time */
	for (i = 0; i < ARRAY_SIZE(int_set); ++i) {
		int_set[i] = rand();
		SETint_set(&s, int_set[i]);
	}
	for (i = 0; i < ARRAY_SIZE(int_set) / 2; ++i) {
retry:
		tmp = rand();
		for (j = 0; j < ARRAY_SIZE(int_set); ++j)
			if (int_set[j] == tmp)
				goto retry;
		SETint_set(&s, tmp);
		SETint_unset(&s, tmp);
		assert(!SETint_isset(&s, tmp));
	}
	for (i = 0; i < ARRAY_SIZE(int_set); ++i)
		assert(SETint_isset(&s, int_set[i]));
	SETint_free(&s);
	printf("OK\n");
}

int
main(void)
{
	test_str();
	test_int();
}
