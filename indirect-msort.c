/* Force fast_permute small cases. */
#undef __SIZEOF_POINTER__
#define __SIZEOF_POINTER__ 4

#define qsort not_qsort
#include "msort.c"
#undef qsort

void qsort(void *b, int n, size_t s, int (*cmp)())
{
    void *arg = NULL;
    void *tmp = malloc(2 * n * sizeof(void *));
    assert(tmp);
    indirect_msort (b, n, s, cmp, arg, tmp);
    free(tmp);
}
