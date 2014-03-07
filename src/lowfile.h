/* Korovin Viktor */
#ifndef LOWFILE_H
#define LOWFILE_H

#include "logging.h"
#include <inttypes.h>
#include <stdio.h>

#define BUF_LEN (64)
#define BUF_LEN_BITS (BUF_LEN*8)

#define IO_WRITE_ERROR (2451)
#define IO_READ_ERROR  (2452)
#define IO_EOF         (2453)

int writeNBytes(FILE *f, int64_t N, char *str, int drop);
int writeInt64(FILE *f, int64_t num, int drop);
int writeChar(FILE *f, char ch, int drop);

int readNBytes(FILE *f, uint64_t N, char *str, size_t *read_bytes);

#endif // LOWFILE_H
