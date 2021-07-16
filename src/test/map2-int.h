MAP_GEN(int, char *, inthash, intcmp, IntMap, int_map)

struct item1 {
    int k;
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

static struct item1 items1_del[] = {
    { .k = 18, .v = "crap val 1" },
    { .k = 6215,   .v = "crap val 2" },
    { .k = 4356,  .v = "crap val 3" },
    { .k = 145,  .v = "crap val 4" },
};

void t_map2_int(void)
{
    IntMap intmap;
    int_map_init(&intmap);

    for (size_t i = 0; i < ARRAY_SIZE(items1_del); ++i) {
        *int_map_put(&intmap, items1_del[i].k) = items1_del[i].v;
        int_map_del(&intmap, items1_del[i].k);
    }
    for (size_t i = 0; i < ARRAY_SIZE(items1_del); ++i)
        assert(!int_map_get(&intmap, items1_del[i].k));

    for (size_t i = 0; i < ARRAY_SIZE(items1); ++i)
        *int_map_put(&intmap, items1[i].k) = items1[i].v;
    for (size_t i = 0; i < ARRAY_SIZE(items1); ++i)
        assert(*int_map_get(&intmap, items1[i].k) == items1[i].v);

    int_map_free(&intmap);
}
