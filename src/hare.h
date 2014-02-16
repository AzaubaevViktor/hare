#ifndef HARE_H
#define HARE_H
#include <inttypes.h>
#include <stdint.h>

typedef struct {
  int argc;
  char *argv[];
  char *env[];
  int8_t keys;
  /* 0x80,0x40, ... , 0x2, 0x1: Ø, Ø, Ø, Add, eXtract, Delete, List, inTegrity check */
  char *archName;
  char *workFiles[];
} Context;

#endif // HARE_H
