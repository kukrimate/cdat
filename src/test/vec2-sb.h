static char *strings[] = {
    "Test string 1",
    "String2\n\raggagagkakjga",
    "{{$$strdata$$}}",
};
static char *concated = "Test string 1String2\n\raggagagkakjga{{$$strdata$$}}";

VEC_GEN(char, StringBuilder, sb)

void t_vec2_char(void)
{
    StringBuilder a;
    sb_init(&a);

    for (size_t i = 0; i < sizeof(strings) / sizeof(char *); ++i)
        for (size_t j = 0; j < strlen(strings[i]); ++j)
            sb_add(&a, strings[i][j]);
    assert(!strncmp(concated, a.arr, a.n));
    sb_free(&a);
}
