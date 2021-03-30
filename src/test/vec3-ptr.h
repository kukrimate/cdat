VEC_GEN(void *, pvoid)

static void *ptrs[] = {
    NULL,
    (void *) 0xDEADBEEF,
    teststructs,
    strings
};

void t_vec3_ptr(void)
{
    VECpvoid a;
    VECpvoid_init(&a);

    for (size_t i = 0; i < sizeof(ptrs) / sizeof(void *); ++i)
        VECpvoid_add(&a, ptrs[i]);
    assert(!memcmp(ptrs, a.arr, sizeof(ptrs)));
    VECpvoid_free(&a);
}
