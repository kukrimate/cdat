SET_GEN(int, inthash, intcmp, int)

void t_set2_int(void)
{
    Set_int s;
    int int_set[1000];

    set_int_init(&s);
    srand(time(NULL)); /* Make sure we get different ints each time */
    for (size_t i = 0; i < ARRAY_SIZE(int_set); ++i) {
        int_set[i] = rand();
        set_int_set(&s, int_set[i]);
    }
    for (size_t i = 0; i < ARRAY_SIZE(int_set) / 2; ++i) {
retry:;
        int tmp = rand();
        for (size_t j = 0; j < ARRAY_SIZE(int_set); ++j)
            if (int_set[j] == tmp)
                goto retry;
        set_int_set(&s, tmp);
        set_int_unset(&s, tmp);
        assert(!set_int_isset(&s, tmp));
    }
    for (size_t i = 0; i < ARRAY_SIZE(int_set); ++i)
        assert(set_int_isset(&s, int_set[i]));
    set_int_free(&s);
}
