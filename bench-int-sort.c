#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <sys/times.h>
#include <math.h>

/* Basic integer type. */
#ifndef T
#define T int
#endif

/* Number of records. */
#ifndef N
#define N (1 << 20)
#endif

/* Each record is S integers. */
#ifndef S
#define S 2
#endif

/* Array of records. */
T a[N * S];

/* Compare by the first integer. */
static int cmp(const void *a, const void *b)
{
    return *(T *) a - *(T *) b;
}

/* Whether the sorintg algorithm is stable. */
#ifndef STABLE
#define STALBE 1
#endif

/* Benchmark array sort. */
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

    /* Dummy system call. */
    getpid();

    time_t ticks = clock();
    qsort(a, N, S * sizeof(T), cmp);
    ticks = clock() - ticks;

#if STABLE
    T n = N;
    bool overflow = (n != N);
#endif

    for (size_t i = 1; i < N; i++) {
	/* First integers are now ordered. */
	assert(a[S * (i - 1)] <= a[S * i]);
#if STABLE
	/* Check stable sort using second integer. */
	if (!overflow)
	assert(a[S * (i - 1)] < a[S * i] ||
	       a[S * (i - 1) + 1] < a[S * i + 1]);
#endif
    }

    return ticks / (double) CLOCKS_PER_SEC;
}

int main(void)
{
    /* At least two runs, at least 5 seconds. */
    double sec = bench();
    size_t iter = 1;
    do {
	sec += bench();
	iter++;
    } while (sec < 5.0);

    /* Mix in user time. */
    struct tms tms;
    times(&tms);
    sec += tms.tms_utime / sysconf(_SC_CLK_TCK);
    sec /= 2.0;

    /*
     * User time in nanoseconds divided by N log N, which would be flat if
     * user time was truly proportional to N log N.  See [C. Eric Wu, Gokul
     * Kandiraju, Pratap Pattnaik. High-Performance Sorting Algorithms on AIX].
     */
    printf("%.3g\n", sec * 1e9 / iter / N / log2(N));
    return 0;
}
