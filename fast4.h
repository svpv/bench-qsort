#ifndef CAT3
#define CAT3_(a, b, c) a ## b ## c
#define CAT3(a, b, c) CAT3_(a, b, c)
#endif

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
  {
#define SIZE 9
#define FUNC CAT3(NAME, 32x, SIZE)
#include HEADER
    FUNC (ARGS);
#undef SIZE
#undef FUNC
  }
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
