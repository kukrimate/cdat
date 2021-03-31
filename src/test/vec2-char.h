static char *strings[] = {
    "Test string 1",
    "String2\n\raggagagkakjga",
    "{{$$strdata$$}}",
};
static char *concated = "Test string 1String2\n\raggagagkakjga{{$$strdata$$}}";

VEC_GEN(char, char)

void t_vec2_char(void)
{
    Vec_char a;
    vec_char_init(&a);

    for (size_t i = 0; i < sizeof(strings) / sizeof(char *); ++i)
        for (size_t j = 0; j < strlen(strings[i]); ++j)
            vec_char_add(&a, strings[i][j]);
    assert(!strncmp(concated, a.arr, a.n));
    vec_char_free(&a);
}
