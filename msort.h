/* An optimized mergesort implementation.
   This file is part of the GNU C Library.
   Copyright (C) 2013 Free Software Foundation, Inc.
   Written by Alexey Tourbin, July 2013.

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

#ifndef _FUNC_H
#define _FUNC_H

/* Copy TYPE memory units.  */
#define COPYU(dest, src, n)			\
  memcpy (dest, src, (n) * sizeof (TYPE))

/* Copy one element.  */
#define COPY1(dest, src)			\
  COPYU(dest, src, SIZE)

/* Swap 2 elements.  */
#define SWAP(a, b)				\
  do						\
    {						\
      TYPE c[SIZE];				\
      COPY1 (c, a);				\
      COPY1 (a, b);				\
      COPY1 (b, c);				\
    }						\
  while (0)

/* Sort 2 elements.  */
#define SORT2(a1, a2)				\
  do						\
    {						\
      if (LESS (a2, a1))			\
	SWAP (a1, a2);				\
    }						\
  while (0)

/* Sort 3 elements.  */
#define SORT3(a1, a2, a3)			\
  do						\
    {						\
      if (LESS (a2, a1))			\
	{					\
	  if (LESS (a3, a2))			\
	    SWAP (a1, a3);			\
	  else					\
	    {					\
	      SWAP (a1, a2);			\
	      if (LESS (a3, a2))		\
		SWAP (a2, a3);			\
	    }					\
	}					\
      else if (LESS (a3, a2))			\
	{					\
	  SWAP (a2, a3);			\
	  if (LESS (a2, a1))			\
	    SWAP (a1, a2);			\
	}					\
    }						\
  while (0)

/* To sort 4 elements, combine SORT3 and INSERT4.  */
#define INSERT4(a1, a2, a3, a4)			\
  do						\
    {						\
      if (LESS (a4, a2))			\
	{					\
	  SWAP (a3, a4);			\
	  SWAP (a2, a3);			\
	  if (LESS (a2, a1))			\
	    SWAP (a1, a2);			\
	}					\
      else					\
	{					\
	  if (LESS (a4, a3))			\
	    SWAP (a3, a4);			\
	}					\
    }						\
  while (0)

/* Loop logic: skip input element.  */
#define SKIP(src, n)				\
  do						\
    {						\
      n--;					\
      src += SIZE;				\
    }						\
  while (0)

/* Loop logic: copy input element.  */
#define COPY(dest, src, n)			\
  do						\
    {						\
      n--;					\
      COPY1 (dest, src);			\
      src += SIZE;				\
      dest += SIZE;				\
    }						\
  while (0)

#endif /* _FUNC_H */

/* Recursive nested function.  */
auto void FUNC (TYPE * a, size_t n);

/* To use this nested function, define the following macros:
   FUNC - function name;
   TYPE - array element type or unit type;
   SIZE - element size, in TYPE units (constant or variable);
   LESS(a, b) - ordering relation;
   TMP - temporary storage.  */
void
FUNC (TYPE * a, size_t n)
{
  /* Handle base case.  */
  switch (n)
    {
    case 0:
    case 1:
      /* Cannot happen.  */
    case 2:
      SORT2 (a, a + SIZE);
      return;
    case 3:
    case 4:
      SORT3 (a, a + SIZE, a + 2 * SIZE);
      if (n == 4)
	INSERT4 (a, a + SIZE, a + 2 * SIZE, a + 3 * SIZE);
      return;
    }

  /* Divide into two halves.  */
  size_t n1 = n / 2;
  size_t n2 = n - n1;
  TYPE *a1 = a;
  TYPE *a2 = a + n1 * SIZE;

  /* Sort each half recursively.  */
  FUNC (a1, n1);
  FUNC (a2, n2);

  /* Merge a1 and a2 back into a.  */
  TYPE *t = (TYPE *) TMP;

  /* Stage 1: skip lesser elements.  */
  while (1)
    if (!LESS (a2, a1))
      {
	SKIP (a1, n1);
	if (n1 == 0)
	  return;
	SKIP (a, n);
      }
    else
      {
	/* Stage 2: merge into tmp.  */
	COPY (t, a2, n2);
	break;
      }

  /* Stage 2 continued.  */
  while (1)
    if (!LESS (a2, a1))
      {
	COPY (t, a1, n1);
	if (n1 == 0)
	  break;
      }
    else
      {
	COPY (t, a2, n2);
	if (n2 == 0)
	  {
	    /* Stage 3: move greater elements.  */
	    a2 -= n1 * SIZE;
	    COPYU (a2, a1, n1 * SIZE);
	    break;
	  }
      }

  /* Stage 4: write back.  */
  COPYU (a, TMP, t - (TYPE *) TMP);
}
