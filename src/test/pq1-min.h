#define LT(x, y) ((x) < (y))
PQ_GEN(int, LT, imin)

void t_pq1_min(void)
{
    PQimin min_heap;
    PQimin_init(&min_heap);

    // Add 100k random integers
    srand(time(NULL));
    for (size_t cnt = 100000; cnt; --cnt) {
        PQimin_push(&min_heap, rand() % 100);
    }

    // Make sure we get everything back in sorted order
    int prev = PQimin_pop(&min_heap);
    while (min_heap.n) {
        int cur = PQimin_pop(&min_heap);
        assert(prev <= cur);
        prev = cur;
    }

    // Free queue
    PQimin_free(&min_heap);
}
