#include <sys/stat.h>
#include <stdint.h>
#include "uart.h"

// HEAP management
extern uint32_t _end;
extern uint32_t _estack;
static uint8_t *heap_end;

void *_sbrk(int incr)
{
    if (heap_end == 0)
        heap_end = (uint8_t *)&_end;

    uint8_t *prev = heap_end;
    if ((heap_end + incr) > (uint8_t *)&_estack)
        return (void *) -1;

    heap_end += incr;
    return (void *)prev;
}

// Redirect printf → UART
int _write(int fd, char *ptr, int len)
{
    for (int i = 0; i < len; i++)
        UART2_SendChar(ptr[i]);
    return len;
}

int _close(int fd) { return -1; }
int _fstat(int fd, struct stat *st) { st->st_mode = S_IFCHR; return 0; }
int _isatty(int fd) { return 1; }
int _lseek(int fd, int ptr, int dir) { return 0; }
int _read(int fd, char *ptr, int len) { return 0; }

int _kill(int pid, int sig) { return -1; }
int _getpid(void) { return 1; }

void abort(void)
{
    while (1);
}

