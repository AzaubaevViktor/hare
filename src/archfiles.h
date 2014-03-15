#ifndef ADDFILES2ARCH_H
#define ADDFILES2ARCH_H

#include "logging.h"
#include "fileinfo.h"
#include "lowfile.h"
#include <inttypes.h>
#include <stdio.h>

#define SIGNATURE "\x07\x1F\x0E\x58"
#define SIGNATURE_LEN (4)
#define SIGNATURE_ERROR (6443)
#define MEMORY_ALLOCATE_ERROR (9165)
#define INT64SIZE (sizeof(int64_t))

typedef struct {
  FileInfo *fileInfo;
  int64_t dataSize;
  char endUnusedBits;
  char flags;
  int64_t haffTreeSize;
  char *haffTree;
  int64_t HeaderCheckSum;
} ArchFileInfo;

int writeFileHeader(FILE *f, ArchFileInfo *info, int drop);

int readHeader(FILE *f, ArchFileInfo *file);

#endif // ADDFILES2ARCH_H
