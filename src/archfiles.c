/* 2014, Korovin Viktor <sarvash.inc@gmail.com> Потоковая запись файла */
#include "archfiles.h"


int writeFileHeader(FILE *f, ArchFileInfo *info, int drop) {
  FileInfo *fInfo = NULL;
  IO("Write file header");
  fInfo = info->fileInfo;
  writeNBytes(f, SIGNATURE_LEN, SIGNATURE, 0);
  writeInt64(f, fInfo->sizeName, 0);
  writeNBytes(f, fInfo->sizeName, fInfo->name, 0);
  writeInt64(f, fInfo->timeLastModification, 0);
  writeInt64(f, fInfo->size, 0);
  writeInt64(f, info->dataSize, 0);
  writeChar(f, info->endUnusedBits, 0);
  writeChar(f, info->flags, 0);
  writeInt64(f, info->haffTreeSize, 0);
  writeNBytes(f, info->haffTreeSize, info->haffTree, 0);
  writeInt64(f, info->HeaderCheckSum, drop);

  return 0;
}

size_t getHeaderLen(ArchFileInfo *info) {
  return SIGNATURE_LEN + info->fileInfo->sizeName + info->haffTreeSize + sizeof(char)*2 + INT64SIZE*(6);
}

int writeData(FILE *f, int64_t size, void *data) {
  return writeNBytes(f, size, data, 0);
}

int readHeader(FILE *f, ArchFileInfo *file) {
  char signature[SIGNATURE_LEN] = "";
  int64_t fileNameLen = 0;
  size_t read_bytes = 0;
  FileInfo *fInfo = NULL;
  IO("Read signature");

  fInfo = file->fileInfo;

  readNBytes(f, SIGNATURE_LEN, signature, &read_bytes);
  if (memcmp(signature, SIGNATURE, SIGNATURE_LEN)) {
    printf("%d\n", memcmp(signature, SIGNATURE, SIGNATURE_LEN));
    WARNING("Signature does not match");
    return SIGNATURE_ERROR;
  }

  readInt64(f, &fileNameLen, &read_bytes);
  fInfo->name = malloc(fileNameLen+1);
  if (NULL == fInfo->name) {
    MEMORY("Memory allocate error!");
    return MEMORY_ALLOCATE_ERROR;
  }
  readNBytes(f, fileNameLen, fInfo->name, &read_bytes);
  fInfo->name[fileNameLen] = 0;

  readInt64(f, &(fInfo->timeLastModification), &read_bytes);

  readInt64(f, &(fInfo->size), &read_bytes);

  readInt64(f, &(file->dataSize), &read_bytes);

  readChar(f, file->endUnusedBits, &read_bytes);

  readChar(f, file->flags, &read_bytes);

  return 0;
}
