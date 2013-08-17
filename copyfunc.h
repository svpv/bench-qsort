#include <stdint.h>
#include <string.h>

#define COPYFUNC(b, s)							\
  static inline void							\
  copy ## b ## x ## s (uint ## b ## _t *dest, uint ## b ## _t *src)	\
  {									\
    memcpy(dest, src, b / 8 * s);					\
  }

COPYFUNC (32, 9)
