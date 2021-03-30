#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <djb2.h>
#include <map.h>
#include <pq.h>
#include <vec.h>
#include <set.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

#include "map1-str.h"
#include "map2-int.h"
#include "pq1-min.h"
#include "pq2-max.h"
#include "vec1-stru.h"
#include "vec2-char.h"
#include "vec3-ptr.h"
#include "set1-str.h"
#include "set2-int.h"

typedef struct {
    const char *msg;
    void (*test_fn)(void);
} TestDef;

static TestDef tests[] = {
    { "string map" , t_map1_str , },
    { "int map"    , t_map2_int , },
    { "min heap"   , t_pq1_min  , },
    { "max heap"   , t_pq2_max  , },
    { "struct vec" , t_vec1_stru, },
    { "char vec"   , t_vec2_char, },
    { "pointer vec", t_vec3_ptr , },
    { "string set" , t_set1_str , },
    { "int set"    , t_set2_int , },
};

int main(void)
{
    for (size_t i = 0; i < ARRAY_SIZE(tests); ++i) {
        printf("Testing %s... ", tests[i].msg);
        tests[i].test_fn();
        printf("OK\n");
    }
}

