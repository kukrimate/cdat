VEC_GEN(void *, pvoid)

static void *ptrs[] = {
    NULL,
    (void *) 0xDEADBEEF,
    teststructs,
    strings
};

void t_vec3_ptr(void)
{
    Vec_pvoid a;
    vec_pvoid_init(&a);

    for (size_t i = 0; i < sizeof(ptrs) / sizeof(void *); ++i)
        vec_pvoid_add(&a, ptrs[i]);
    assert(!memcmp(ptrs, a.arr, sizeof(ptrs)));
    vec_pvoid_free(&a);
}
