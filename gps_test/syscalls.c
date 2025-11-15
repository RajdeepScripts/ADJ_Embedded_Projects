#include <sys/types.h>
#include <errno.h>

extern char end;      // Provided by linker script
static char *heap_end;

void *_sbrk(int incr)
{
    if (heap_end == 0)
        heap_end = &end;

    char *prev = heap_end;
    heap_end += incr;
    return (void *)prev;
}

