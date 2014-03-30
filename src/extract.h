#ifndef EXTRACT_H
#define EXTRACT_H

#include <stdlib.h>
#include <stdio.h>
#include <logging.h>
#include <archfiles.h>

#define FSEEK_ERROR (7145)
#define FILE_OPEN_ERROR (8155)
#define BUF_SIZE (5)

char *decoding(char *bytes, size_t lenBit, size_t *returnBits);
int extract(FILE *f, size_t offset, ArchFileInfo *info, char *fileName);

#endif // EXTRACT_H
