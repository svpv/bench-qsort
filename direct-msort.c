#define BENCH_INDIRECT 1
#define qsort not_qsort
#include "msort.c"
#undef qsort

void qsort(void *b, int n, size_t s, int (*cmp)())
{
    void *arg = NULL;
    void *tmp = malloc(n * s);
    assert(tmp);
    if (s <= FAST_MAX_BYTES && FAST_P(b, s))
	fast_msort (b, n, s, cmp, arg, tmp);
    else
	generic_msort (b, n, s, cmp, arg, tmp);
    free(tmp);
}
