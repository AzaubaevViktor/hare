/* 2014, Korovin Viktor <sarvash.inc@gmail.com> Потоковая запись файла */
#include "archfiles.h"


int writeFileHeader(FILE *f, ArchFileInfo *info) {
  FileInfo *fInfo = NULL;
  IO("Write file header");
  fInfo = info->fileInfo;
  writeNBytes(f, SIGNATURE_LEN, SIGNATURE);
  writeInt64(f, fInfo->sizeName);
  writeNBytes(f, fInfo->sizeName, fInfo->name);
  writeInt64(f, fInfo->timeLastModification);
  writeInt64(f, fInfo->size);
  writeInt64(f, info->dataSize);
  writeChar(f, info->endUnusedBits);
  writeChar(f, info->flags);
  writeInt64(f, info->haffTreeSize);
  writeNBytes(f, info->haffTreeSize, info->haffTree);
  writeInt64(f, info->HeaderCheckSum);

  return 0;
}

size_t getHeaderLen(ArchFileInfo *info) {
  return SIGNATURE_LEN + info->fileInfo->sizeName + info->haffTreeSize + sizeof(char)*2 + INT64SIZE*(6);
}

int writeData(FILE *f, int64_t size, void *data) {
  return writeNBytes(f, size, data);
}

//TODO: Добавить проброс ошибок наружу, нормальный обработчик EOF

int readHeader(FILE *f, ArchFileInfo *info) {
  char signature[SIGNATURE_LEN] = "";
  int64_t fileNameLen = 0;
  size_t read_bytes = 0;
  FileInfo *fInfo = NULL;
  LOGGING_FUNC_START;
  IO(L"Read signature");

  fInfo = info->fileInfo;

  readNBytes(f, SIGNATURE_LEN, signature, &read_bytes);
  if (memcmp(signature, SIGNATURE, SIGNATURE_LEN)) {
    printf(":archfiles.c:44: %d\n", memcmp(signature, SIGNATURE, SIGNATURE_LEN));
    WARNING(L"Signature does not match");
    LOGGING_FUNC_STOP;
    return SIGNATURE_ERROR;
  }

  readInt64(f, &fileNameLen, &read_bytes);

  if (NULL == (fInfo->name = malloc(fileNameLen+1))) {
    MEMORY(L"Memory allocate error!");
    LOGGING_FUNC_STOP;
    return MEMORY_ALLOCATE_ERROR;
  }
  readNBytes(f, fileNameLen, fInfo->name, &read_bytes);
  fInfo->name[fileNameLen] = 0;

  readInt64(f, &(fInfo->timeLastModification), &read_bytes);

  readInt64(f, &(fInfo->size), &read_bytes);

  readInt64(f, &(info->dataSize), &read_bytes);

  readChar(f, &(info->endUnusedBits), &read_bytes);

  readChar(f, &(info->flags), &read_bytes);

  readInt64(f, &(info->haffTreeSize), &read_bytes);

  if (NULL == (info->haffTree = malloc(info->haffTreeSize + 1))) {
    MEMORY(L"Memory allocate error!");
    LOGGING_FUNC_STOP;
    return MEMORY_ALLOCATE_ERROR;
  }
  readNBytes(f, info->haffTreeSize, info->haffTree, &read_bytes);

  readInt64(f, &(info->HeaderCheckSum), &read_bytes);

  LOGGING_FUNC_STOP;
  return 0;
}
