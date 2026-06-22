#ifndef MEM_INCLUDED
#define MEM_INCLUDED
#include <stdlib.h>
#include "except.h"

// exceptions
extern const Except_T Mem_Failed;
// functions
extern void *Mem_alloc(long nbytes, const char *file, int line);
extern void *Mem_calloc(long count, long nbytes, const char *file, int line);
extern void Mem_free(void *ptr, const char *file, int line);
extern void *Mem_resize(void *ptr, long nbytes, const char *file, int line);

// macros
#define ALLOC(nbytes) Mem_alloc((nbytes), __FILE__, __LINE__)
#define CALLOC(count, nbytes) Mem_calloc((count), (nbytes), __FILE__, __LINE__)
#define NEW(p) ((p) = ALLOC((long)sizeof *(p)))
#define NEWO(p) ((p) = CALLOC(1, (long)sizeof *(p)))
#define FREE(p) ((void)(Mem_free((p), __FILE__, __LINE__), (ptr) = 0))
#define RESIZE(ptr, nbytes) ((ptr) = Mem_resize((ptr), (nbytes), __FILE__, __LINE__))


#endif
