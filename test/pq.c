/*
 * Priority queue tests
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pq.h>

#define LT(x, y) ((x) < (y))
#define GT(x, y) ((x) > (y))

PQ_GEN(int, LT, imin)
PQ_GEN(int, GT, imax)

static void test_min()
{
    PQimin min_heap;
    size_t cnt;
    int prev, cur;

    printf("Running min heap tests... ");

    // Initialize queue
    PQimin_init(&min_heap);

    // Add 100k random integers
    srand(time(NULL));
    for (cnt = 100000; cnt; --cnt) {
        PQimin_push(&min_heap, rand() % 100);
    }

    // Make sure we get everything back in sorted order
    prev = PQimin_pop(&min_heap);
    while (min_heap.n) {
        cur = PQimin_pop(&min_heap);
        assert(prev <= cur);
        prev = cur;
    }

    // Free queue
    PQimin_free(&min_heap);

    printf("OK\n");
}

static void test_max()
{
    PQimax max_heap;
    size_t cnt;
    int prev, cur;

    printf("Running max heap tests... ");

    // Initialize queue
    PQimax_init(&max_heap);

    // Add 100k random integers
    srand(time(NULL));
    for (cnt = 100000; cnt; --cnt) {
        PQimax_push(&max_heap, rand() % 100);
    }

    // Make sure we get everything back in sorted order
    prev = PQimax_pop(&max_heap);
    while (max_heap.n) {
        cur = PQimax_pop(&max_heap);
        assert(prev >= cur);
        prev = cur;
    }

    // Free queue
    PQimax_free(&max_heap);

    printf("OK\n");
}

int main()
{
    test_min();
    test_max();
}
