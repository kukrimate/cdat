PQ_GEN(int, -intcmp, MaxHeap, max_heap)

void t_pq2_max(void)
{
    MaxHeap max_heap;
    max_heap_init(&max_heap);

    // Add 100k random integers
    srand(time(NULL));
    for (size_t cnt = 100000; cnt; --cnt) {
        max_heap_push(&max_heap, rand() % 100);
    }

    // Make sure we get everything back in sorted order
    int prev = max_heap_pop(&max_heap);
    while (max_heap.n) {
        int cur = max_heap_pop(&max_heap);
        assert(prev >= cur);
        prev = cur;
    }

    // Free queue
    max_heap_free(&max_heap);
}
