PQ_GEN(int, -intcmp, imax)

void t_pq2_max(void)
{
    Pq_imax max_heap;
    pq_imax_init(&max_heap);

    // Add 100k random integers
    srand(time(NULL));
    for (size_t cnt = 100000; cnt; --cnt) {
        pq_imax_push(&max_heap, rand() % 100);
    }

    // Make sure we get everything back in sorted order
    int prev = pq_imax_pop(&max_heap);
    while (max_heap.n) {
        int cur = pq_imax_pop(&max_heap);
        assert(prev >= cur);
        prev = cur;
    }

    // Free queue
    pq_imax_free(&max_heap);
}
