#ifndef ADDFILES2ARCH_H
#define ADDFILES2ARCH_H

#include "logging.h"
#include "fileinfo.h"
#include "lowfile.h"
#include "crc.h"
#include <inttypes.h>
#include <stdio.h>

#define SIGNATURE "\x07\x1F\x0E\x58"
#define SIGNATURE_LEN (4)
#define SIGNATURE_ERROR (6443)
#define MEMORY_ALLOCATE_ERROR (9165)
#define HASH_HEADER_CHECK_ERROR (3270)
#define MKDIR_ERROR (8172)
#define INT64SIZE (sizeof(int64_t))

#define ceil8(num) (((num) / 8) + !!((num) % 8))

typedef struct {
  FileInfo *fileInfo;
  int64_t dataSize;
  char endUnusedBits;
  char flags;
  int64_t haffTreeSize;
  char *haffTree;
  crc HeaderCheckSum;
} ArchFileInfo;

int writeFileHeader(FILE *f, ArchFileInfo *info);

size_t getHeaderLen(ArchFileInfo *info);

/* return HASH_HEADER_CHECK_ERROR if checksum doesn't match */
int readHeader(FILE *f, ArchFileInfo *file);

#endif // ADDFILES2ARCH_H
