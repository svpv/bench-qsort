#ifndef COPYSWITH_H
#define COPYSWITH_H

#define CAT2_(a, b) a ## b
#define CAT2(a, b) CAT2_(a, b)

#define CAT3_(a, b, c) a ## b ## c
#define CAT3(a, b, c) CAT3_(a, b, c)

#endif

/* Function with specialized copy callback. */
void (*copyfunc) ();
#define CFUNC CAT2(NAME, C)
#define FUNC CFUNC
#define TYPE char
#define SIZE s
#define COPY1(dest, src) copyfunc (dest, src, s)
#include HEADER
#undef FUNC
#undef TYPE
#undef SIZE
#undef COPY1

/* Odd cases.  */
#define TYPE uint32_t

#ifndef SKIP_CASE1
case 1:
  {
#define SIZE 1
#define FUNC CAT3(NAME, 32x, SIZE)
#include HEADER
    FUNC (ARGS);
#undef SIZE
#undef FUNC
  }
  break;
#endif

#ifndef SKIP_CASE3
case 3:
  {
#define SIZE 3
#define FUNC CAT3(NAME, 32x, SIZE)
#include HEADER
    FUNC (ARGS);
#undef SIZE
#undef FUNC
  }
  break;
#endif

#ifndef SKIP_CASE5
case 5:
  {
#define SIZE 5
#define FUNC CAT3(NAME, 32x, SIZE)
#include HEADER
    FUNC (ARGS);
#undef SIZE
#undef FUNC
  }
  break;
#endif

#ifndef SKIP_CASE7
case 7:
  {
#define SIZE 7
#define FUNC CAT3(NAME, 32x, SIZE)
#include HEADER
    FUNC (ARGS);
#undef SIZE
#undef FUNC
  }
  break;
#endif

#ifndef SKIP_CASE9
case 9:
#ifndef COPY32
  {
#define SIZE 9
#define FUNC CAT3(NAME, 32x, SIZE)
#include HEADER
    FUNC (ARGS);
#undef SIZE
#undef FUNC
  }
#else
  copyfunc = copy32x9;
  CFUNC(ARGS);
#endif
  break;
#endif

/* Even cases.  */
#undef TYPE

#ifndef SKIP_CASE2
case 2:
  if (USE64)
    {
#define SIZE 1
#define TYPE uint64_t
#define FUNC CAT3(NAME, 64x, SIZE)
#include HEADER
      FUNC (ARGS);
#undef SIZE
#undef TYPE
#undef FUNC
    }
  else
    {
#define SIZE 2
#define TYPE uint32_t
#define FUNC CAT3(NAME, 32x, SIZE)
#include HEADER
      FUNC (ARGS);
#undef SIZE
#undef TYPE
#undef FUNC
    }
  break;
#endif

#ifndef SKIP_CASE4
case 4:
  if (USE64)
    {
#define SIZE 2
#define TYPE uint64_t
#define FUNC CAT3(NAME, 64x, SIZE)
#include HEADER
      FUNC (ARGS);
#undef SIZE
#undef TYPE
#undef FUNC
    }
  else
    {
#define SIZE 4
#define TYPE uint32_t
#define FUNC CAT3(NAME, 32x, SIZE)
#include HEADER
      FUNC (ARGS);
#undef SIZE
#undef TYPE
#undef FUNC
    }
  break;
#endif

#ifndef SKIP_CASE6
case 6:
  if (USE64)
    {
#define SIZE 3
#define TYPE uint64_t
#define FUNC CAT3(NAME, 64x, SIZE)
#include HEADER
      FUNC (ARGS);
#undef SIZE
#undef TYPE
#undef FUNC
    }
  else
    {
#define SIZE 6
#define TYPE uint32_t
#define FUNC CAT3(NAME, 32x, SIZE)
#include HEADER
      FUNC (ARGS);
#undef SIZE
#undef TYPE
#undef FUNC
    }
  break;
#endif

#ifndef SKIP_CASE8
case 8:
  if (USE64)
    {
#define SIZE 4
#define TYPE uint64_t
#define FUNC CAT3(NAME, 64x, SIZE)
#include HEADER
      FUNC (ARGS);
#undef SIZE
#undef TYPE
#undef FUNC
    }
  else
    {
#define SIZE 8
#define TYPE uint32_t
#define FUNC CAT3(NAME, 32x, SIZE)
#include HEADER
      FUNC (ARGS);
#undef SIZE
#undef TYPE
#undef FUNC
    }
  break;
#endif

#ifndef SKIP_CASE10
case 10:
  if (USE64)
    {
#define SIZE 5
#define TYPE uint64_t
#define FUNC CAT3(NAME, 64x, SIZE)
#include HEADER
      FUNC (ARGS);
#undef SIZE
#undef TYPE
#undef FUNC
    }
  else
    {
#define SIZE 10
#define TYPE uint32_t
#define FUNC CAT3(NAME, 32x, SIZE)
#include HEADER
      FUNC (ARGS);
#undef SIZE
#undef TYPE
#undef FUNC
    }
  break;
#endif
