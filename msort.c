/* An alternative to qsort, with an identical interface.
   This file is part of the GNU C Library.
   Copyright (C) 1992,95-97,99,2000,01,02,04,07 Free Software Foundation, Inc.
   Written by Mike Haertel, September 1988.
   Later improvements by Ulrich Drepper, Jakub Jelinek, and Alexey Tourbin.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <alloca.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <atomic.h>
#include <bits/wordsize.h>

/* CPU register size used for copying.  */
#ifdef __x86_64__
#define REGSIZE 64
#else
#define REGSIZE __WORDSIZE
#endif

/* Check if buffer is aligned.  */
#if _STRING_ARCH_unaligned
#define ALIGNED(b, n) 1
#else
#define ALIGNED(b, n) __builtin_expect ((uintptr_t)(b) % (n) == 0, 1)
#endif

/* Maximum element size handled by the fast routine.
   Larger elements must be handled with indirect sort.  */
#if REGSIZE > 32 && _STRING_ARCH_unaligned
#define FAST_MAX_BYTES (4 * 10)
#else
#define FAST_MAX_BYTES (4 * 8)
#define SKIP_CASE9
#define SKIP_CASE10
#endif

/* Further check if fast routine is applicable.  */
#define FAST_P(b, s) __builtin_expect ((s) % 4 == 0 && ALIGNED (b, 4), 1)

/* Fast routine for multiple of 4 sizes. */
static void
fast_msort (void *b, size_t n, size_t s,
	    __compar_d_fn_t cmp, void *arg, void *tmp)
{
  /* Macros for msort.h.  */
#define LESS(a, b) (cmp(a, b, arg) < 0)
#define TMP tmp

  switch (s / 4)
    {
#define NAME msort
#define HEADER "msort.h"
#define ARGS b, n
#define USE64 (REGSIZE > 32 && ALIGNED(b, 8))
#include "fast4.h"
      /* Cases inserted here.  */
#undef NAME
#undef HEADER
#undef ARGS
#undef USE64
    default:
      assert (!"cannot happen");
      break;
    }
}

/* Maximum element size handled by the generic routine.
   Larger elements should be handled with indirect sort.  */
#if REGSIZE > 32
#define GENERIC_MAX_BYTES (2 * 15)
#else
#define GENERIC_MAX_BYTES (2 * 7)
#endif

/* Generic routine for odd sizes. */
static void
generic_msort (void *b, size_t n, size_t s,
	       __compar_d_fn_t cmp, void *arg, void *tmp)
{
#define FUNC msortc
#define TYPE char
#define SIZE s
#include "msort.h"
  FUNC (b, n);
#undef FUNC
#undef TYPE
#undef SIZE

#undef LESS
#undef TMP
}

/* Fast permutation for multiple of 4 sizes. */
static void
fast_permute (void *b, size_t n, size_t s, void *p)
{
  switch (s / 4)
    {
#define SKIP_CASE1
#define SKIP_CASE2

      /* Small cases are also possible due to malloc failure.  */
#if REGSIZE > 32 && __SIZEOF_POINTER__ > 4
#define SKIP_CASE3
#define SKIP_CASE4
#endif

#define NAME permute
#define HEADER "permute.h"
#define ARGS b, n, p
#define USE64 (REGSIZE > 32 && ALIGNED(b, 8))
#include "fast4.h"
      /* Cases inserted here.  */
#undef NAME
#undef HEADER
#undef ARGS
#undef USE64

#undef SKIP_CASE1
#undef SKIP_CASE2
#if REGSIZE > 32 && __SIZEOF_POINTER__ > 4
#undef SKIP_CASE3
#undef SKIP_CASE4
#endif
    default:
      assert (!"cannot happen");
      break;
    }
}

/* Generic permutation for odd sizes.  */
static void
generic_permute (void *b, size_t n, size_t s, void *p)
{
#define FUNC permutec
#define TYPE char
#define SIZE s
#include "permute.h"
  FUNC (b, n, p);
#undef FUNC
#undef TYPE
#undef SIZE
}

/* Indirect sort routine.  */
static void
indirect_msort (void *b, size_t n, size_t s,
		__compar_d_fn_t cmp, void *arg, void *tmp)
{
  char *ip = (char *) b;
  void **tp = (void **) (tmp + n * sizeof (void *));
  void **t = tp;
  void *tmp_storage = (void *) (tp + n);

  /* Make an array of pointers.  */
  while ((void *) t < tmp_storage)
    {
      *t++ = ip;
      ip += s;
    }

  /* LESS will now dereference its args.  */
#define LESS(a, b) (cmp(*(a), *(b), arg) < 0)
#define TMP tmp

#define FUNC msorti
#define TYPE void *
#define SIZE 1
#include "msort.h"
  FUNC (tp, n);
#undef FUNC
#undef TYPE
#undef SIZE

#undef LESS
#undef TMP

  if (s <= FAST_MAX_BYTES && FAST_P (b, s))
    fast_permute (b, n, s, tp);
  else
    generic_permute (b, n, s, tp);
}

/* Check if element copying in fast routine is somewhat expensive.
   Copying is "expensive" if it takes more than 2 register moves,
   as compared to pointer copying which requires only 1 move.  */
static bool
expensive_regmoves (void *b, size_t s)
{
#if REGSIZE == 32
  (void) b;
  return s > 8;
#elif _STRING_ARCH_unaligned
  (void) b;
  return s > 16;
#else
  if (s == 16)
    return !ALIGNED (b, 8);
  return s > 8;
#endif
}

/* Check if the working set fits into CPU cache. */
static bool
fits_into_cache (size_t size)
{
  static size_t cache_size;

  if (cache_size == 0)
    {
      long ret = __sysconf (_SC_LEVEL2_CACHE_SIZE);
      cache_size = (ret > 0) ? ret : (256 << 10);
    }

  return size <= cache_size;
}

/* Check if indirect sort should be used.  */
static bool
indirect_p (void *b, size_t n, size_t s)
{
  if (s > FAST_MAX_BYTES)
    return true;

  if (FAST_P (b, s))
    {
      if (!expensive_regmoves (b, s))
	return false;
    }
  else if (s > GENERIC_MAX_BYTES)
    return true;

  size_t working_set_size = n * s + n * sizeof (void *);
  return fits_into_cache (working_set_size);
}

static void *
try_malloc (size_t size)
{
  /* We should avoid allocating too much memory since this might
     have to be backed up by swap space.  */
  static long int phys_pages;
  static int pagesize;

  if (pagesize == 0)
    {
      phys_pages = __sysconf (_SC_PHYS_PAGES);

      if (phys_pages == -1)
	/* Error while determining the memory size.  So let's assume
	   there is enough memory.  Otherwise the implementer should
	   provide a complete implementation of the `sysconf' function.  */
	phys_pages = (long int) (~0ul >> 1);

      /* The following determines that we will never use more than
         a quarter of the physical memory.  */
      phys_pages /= 4;

      /* Make sure phys_pages is written to memory.  */
      atomic_write_barrier ();

      pagesize = __sysconf (_SC_PAGESIZE);
    }

  /* Just a comment here.  We cannot compute
     phys_pages * pagesize
     and compare the needed amount of memory against this value.
     The problem is that some systems might have more physical
     memory then can be represented with a `size_t' value (when
     measured in bytes.  */

  /* If the memory requirements are too high don't allocate memory.  */
  if (size / pagesize > (size_t) phys_pages)
    return NULL;

  int save = errno;
  void *tmp = malloc (size);
  __set_errno (save);
  return tmp;
}

void
qsort_r (void *b, size_t n, size_t s, __compar_d_fn_t cmp, void *arg)
{
  if (n <= 1)
    return;
  assert (b);
  assert (s);
  assert (cmp);

  int indirect = indirect_p (b, n, s);

  size_t indirect_size = 2 * n * sizeof (void *);
  size_t size = indirect ? indirect_size : n * s;

  void *tmp;
  int use_alloca = __libc_use_alloca (size);

  if (use_alloca)
    tmp = __alloca (size);
  else
    {
      tmp = try_malloc (size);
      if (tmp == NULL && size > indirect_size)
	{
	  indirect = 1;
	  size = indirect_size;
	  tmp = try_malloc (size);
	}
      if (tmp == NULL)
	{
	  /* Couldn't get space, so use the slower algorithm
	     that doesn't need a temporary array.  */
	  _quicksort (b, n, s, cmp, arg);
	  return;
	}
    }

  if (indirect)
    indirect_msort (b, n, s, cmp, arg, tmp);
  else if (FAST_P (b, s))
    fast_msort (b, n, s, cmp, arg, tmp);
  else
    generic_msort (b, n, s, cmp, arg, tmp);

  if (!use_alloca)
    free (tmp);
}
libc_hidden_def (qsort_r)

void qsort (void *b, size_t n, size_t s, __compar_fn_t cmp)
{
  return qsort_r (b, n, s, (__compar_d_fn_t) cmp, NULL);
}
libc_hidden_def (qsort)
