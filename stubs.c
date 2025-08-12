#include <stdlib.h>
#include <sys/stat.h>
#include "sched.h"

extern volatile tcb_t *tcb;

void *_sbrk(int incr) {
  unsigned char *heap = tcb->sbreak;
  unsigned char *prev_heap;

  prev_heap = heap;

  heap += incr;
  return prev_heap;
}

int _close(int file) {
  return -1;
}

int _fstat(int file, struct stat *st) {
  return 0;
}

int _isatty(int file) {
  return 1;
}

int _lseek(int file, int ptr, int dir) {
  return 0;
}

void _exit(int status) {
  __asm("BKPT #0");
}

void _kill(int pid, int sig) {
  return;
}

int _getpid(void) {
  return -1;
}

int _open(const char *name, int flags, int mode) {
  return -1;
}

int _write (int file, char * ptr, int len) {
  return -1;
}

int _read (int file, char * ptr, int len) {
  return -1;
}