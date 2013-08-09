#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#ifndef T
#define T int
#endif

/* compare by the first integer */
static int cmp(const void *a, const void *b)
{
    return *(T *) a - *(T *) b;
}

#ifndef N
#define N (1 << 20)
#endif
#ifndef S
#define S 2
#endif
T a[N * S];

/* benchmark array sort */
static double bench(void)
{
    for (size_t i = 0; i < N; i++) {
#if ORD
	/* already sorted */
	a[S * i] = i;
#elif REV
	/* reverse sorted */
	a[S * i] = N - i - 1;
#elif MOD
	/* random in a range */
	a[S * i] = rand() % MOD;
#else
	/* unrestrained random */
	a[S * i] = rand();
#endif
	/* save original order in second integer */
#if S > 1
	a[S * i + 1] = i;
#endif
    }

    time_t ticks = clock();
    qsort(a, N, S * sizeof(T), cmp);
    ticks = clock() - ticks;

    for (size_t i = 1; i < N; i++) {
	/* first integers are now ordered */
	assert(a[S * (i - 1)] <= a[S * i]);
	/* check stable sort using second integer */
#if S > 1
	if (sizeof(T) >= sizeof(int))
	assert(a[S * (i - 1)] < a[S * i] ||
	       a[S * (i - 1) + 1] < a[S * i + 1]);
#endif
    }

    return ticks / (double) CLOCKS_PER_SEC;
}

#ifndef I
#define I 16
#endif

int main(void)
{
    double sec = bench();
    for (size_t i = 1; i < I; i++)
	sec += bench();
    printf("%g\n", sec);
    return 0;
}
