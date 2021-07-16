SET_GEN(const char *, strhash, strcmp, StrSet, str_set)

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

void t_set1_str(void)
{
	StrSet s;
	str_set_init(&s);

	for (size_t i = 0; i < ARRAY_SIZE(str_set); ++i)
		str_set_set(&s, str_set[i]);
	for (size_t i = 0; i < ARRAY_SIZE(str_notset); ++i)
		assert(!str_set_isset(&s, str_notset[i]));
	for (size_t i = 0; i < ARRAY_SIZE(str_set); ++i)
		assert(str_set_isset(&s, str_set[i]));

	for (size_t i = 0; i < ARRAY_SIZE(str_set); ++i)
		str_set_unset(&s, str_set[i]);
	for (size_t i = 0; i < ARRAY_SIZE(str_notset); ++i)
		str_set_set(&s, str_notset[i]);

	str_set_free(&s);
}
