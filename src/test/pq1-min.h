PQ_GEN(int, intcmp, MinHeap, min_heap)

void t_pq1_min(void)
{
    MinHeap min_heap;
    min_heap_init(&min_heap);

    // Add 100k random integers
    srand(time(NULL));
    for (size_t cnt = 100000; cnt; --cnt) {
        min_heap_push(&min_heap, rand() % 100);
    }

    // Make sure we get everything back in sorted order
    int prev = min_heap_pop(&min_heap);
    while (min_heap.n) {
        int cur = min_heap_pop(&min_heap);
        assert(prev <= cur);
        prev = cur;
    }

    // Free queue
    min_heap_free(&min_heap);
}
