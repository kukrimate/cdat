#define GT(x, y) ((x) > (y))
PQ_GEN(int, GT, imax)

void t_pq2_max(void)
{
    PQimax max_heap;
    PQimax_init(&max_heap);

    // Add 100k random integers
    srand(time(NULL));
    for (size_t cnt = 100000; cnt; --cnt) {
        PQimax_push(&max_heap, rand() % 100);
    }

    // Make sure we get everything back in sorted order
    int prev = PQimax_pop(&max_heap);
    while (max_heap.n) {
        int cur = PQimax_pop(&max_heap);
        assert(prev >= cur);
        prev = cur;
    }

    // Free queue
    PQimax_free(&max_heap);
}
