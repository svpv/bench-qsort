/* Fake header file for msort.c.  */
#define __libc_use_alloca(size) ((size) <= 4096)
#define __alloca alloca
#define __sysconf sysconf
#define __set_errno(save) (void)(save)
#define _quicksort(b, n, s, cmp, arg) assert(!"quicksort")
#define atomic_write_barrier()
#define libc_hidden_def(x)
