PQ_GEN(int, intcmp, imin)

void t_pq1_min(void)
{
    Pq_imin min_heap;
    pq_imin_init(&min_heap);

    // Add 100k random integers
    srand(time(NULL));
    for (size_t cnt = 100000; cnt; --cnt) {
        pq_imin_push(&min_heap, rand() % 100);
    }

    // Make sure we get everything back in sorted order
    int prev = pq_imin_pop(&min_heap);
    while (min_heap.n) {
        int cur = pq_imin_pop(&min_heap);
        assert(prev <= cur);
        prev = cur;
    }

    // Free queue
    pq_imin_free(&min_heap);
}
