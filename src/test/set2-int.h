SET_GEN(int, inthash, intcmp, IntSet, int_set)

void t_set2_int(void)
{
    IntSet s;
    int int_set[1000];

    int_set_init(&s);
    srand(time(NULL)); /* Make sure we get different ints each time */
    for (size_t i = 0; i < ARRAY_SIZE(int_set); ++i) {
        int_set[i] = rand();
        int_set_set(&s, int_set[i]);
    }
    for (size_t i = 0; i < ARRAY_SIZE(int_set) / 2; ++i) {
retry:;
        int tmp = rand();
        for (size_t j = 0; j < ARRAY_SIZE(int_set); ++j)
            if (int_set[j] == tmp)
                goto retry;
        int_set_set(&s, tmp);
        int_set_unset(&s, tmp);
        assert(!int_set_isset(&s, tmp));
    }
    for (size_t i = 0; i < ARRAY_SIZE(int_set); ++i)
        assert(int_set_isset(&s, int_set[i]));
    int_set_free(&s);
}
