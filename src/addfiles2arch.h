#ifndef ADDFILES2ARCH_H
#define ADDFILES2ARCH_H

#include "logging.h"
#include <stdio.h>

#define BUF_LEN (8)
#define BUF_LEN_BITS (BUF_LEN*8)

#define IO_WRITE_ERROR (2451)

int writeBytes(FILE *f, unsigned char *buf, int64_t k_bytes);

#endif // ADDFILES2ARCH_H
