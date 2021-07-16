VEC_GEN(void *, PtrVec, ptr_vec)

static void *ptrs[] = {
    NULL,
    (void *) 0xDEADBEEF,
    teststructs,
    strings
};

void t_vec3_ptr(void)
{
    PtrVec a;
    ptr_vec_init(&a);

    for (size_t i = 0; i < sizeof(ptrs) / sizeof(void *); ++i)
        ptr_vec_add(&a, ptrs[i]);
    assert(!memcmp(ptrs, a.arr, sizeof(ptrs)));
    ptr_vec_free(&a);
}
