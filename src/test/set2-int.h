SET_GEN(int, inthash, intcmp, int)

void t_set2_int(void)
{
    SETint s;
    int int_set[1000];

    SETint_init(&s);
    srand(time(NULL)); /* Make sure we get different ints each time */
    for (size_t i = 0; i < ARRAY_SIZE(int_set); ++i) {
        int_set[i] = rand();
        SETint_set(&s, int_set[i]);
    }
    for (size_t i = 0; i < ARRAY_SIZE(int_set) / 2; ++i) {
retry:;
        int tmp = rand();
        for (size_t j = 0; j < ARRAY_SIZE(int_set); ++j)
            if (int_set[j] == tmp)
                goto retry;
        SETint_set(&s, tmp);
        SETint_unset(&s, tmp);
        assert(!SETint_isset(&s, tmp));
    }
    for (size_t i = 0; i < ARRAY_SIZE(int_set); ++i)
        assert(SETint_isset(&s, int_set[i]));
    SETint_free(&s);
}
