#ifndef ADDFILES2ARCH_H
#define ADDFILES2ARCH_H

#include "logging.h"
#include "fileinfo.h"
#include <inttypes.h>
#include <stdio.h>

#define BUF_LEN (8)
#define BUF_LEN_BITS (BUF_LEN*8)

#define IO_WRITE_ERROR (2451)

#define SIGNATURE "\x07\x1F\x0E\x58"
#define SIGNATURE_LEN (4)

int writeBytes(FILE *f, char *buf, int drop);
int writeFileHeader(FILE *f,\
                    FileInfo *file,\
                    int64_t archSize,\
                    char endUnusedBits,\
                    char flags,\
                    int64_t haffTreeSize,\
                    char *haffTree,\
                    int64_t HeaderCheckSum,\
                    int drop);

#endif // ADDFILES2ARCH_H
