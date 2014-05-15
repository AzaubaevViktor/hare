/* 2014, Korovin Viktor <sarvash.inc@gmail.com> Потоковая запись файла */
#include "archfiles.h"


#define ERROR_TEST(func) \
  (_error = (func));\
  {if (_error) {\
    WARNING(L"function return error `%d`", _error);\
    LOGGING_FUNC_STOP;\
    return _error;\
  }}


int writeFileHeader(FILE *f, ArchFileInfo *info) {
  FileInfo *fInfo = NULL;
  int _error = 0;
  IO("Write file header");
  fInfo = info->fileInfo;
  ERROR_TEST(writeNBytes(f, SIGNATURE_LEN, SIGNATURE));
  ERROR_TEST(writeInt64(f, fInfo->sizeName));
  ERROR_TEST(writeNBytes(f, fInfo->sizeName, fInfo->name));
  ERROR_TEST(writeInt64(f, fInfo->timeLastModification));
  ERROR_TEST(writeInt64(f, fInfo->size));
  ERROR_TEST(writeInt64(f, info->dataSize));
  ERROR_TEST(writeChar(f, info->endUnusedBits));
  ERROR_TEST(writeChar(f, info->flags));
  ERROR_TEST(writeInt64(f, info->haffTreeSize));
  ERROR_TEST(writeNBytes(f, info->haffTreeSize / 8, info->haffTree));
  ERROR_TEST(writeInt64(f, info->HeaderCheckSum));

  return 0;
}

size_t getHeaderLen(ArchFileInfo *info) {
  return SIGNATURE_LEN + info->fileInfo->sizeName + info->haffTreeSize / 8 + sizeof(char)*2 + INT64SIZE*(6);
}

int writeData(FILE *f, int64_t size, void *data) {
  return writeNBytes(f, size, data);
}


int readHeader(FILE *f, ArchFileInfo *info) {
  char signature[SIGNATURE_LEN] = "";
  int64_t fileNameLen = 0;
  size_t read_bytes = 0;
  FileInfo *fInfo = NULL;
  int _error = 0;
  LOGGING_FUNC_START;
  IO(L"Read signature");

  fInfo = info->fileInfo;

  ERROR_TEST(readNBytes(f, SIGNATURE_LEN, signature, &read_bytes));

  if (memcmp(signature, SIGNATURE, SIGNATURE_LEN)) {
    printf(":archfiles.c:44: %d\n", memcmp(signature, SIGNATURE, SIGNATURE_LEN));
    WARNING(L"Signature does not match");
    LOGGING_FUNC_STOP;
    return SIGNATURE_ERROR;
  }

  ERROR_TEST(readInt64(f, &fileNameLen, &read_bytes));

  if (NULL == (fInfo->name = malloc(fileNameLen+1))) {
    MEMORY(L"Memory allocate error!");
    LOGGING_FUNC_STOP;
    return MEMORY_ALLOCATE_ERROR;
  }
  ERROR_TEST(readNBytes(f, fileNameLen, fInfo->name, &read_bytes));
  fInfo->name[fileNameLen] = 0;

  ERROR_TEST(readInt64(f, &(fInfo->timeLastModification), &read_bytes));
  ERROR_TEST(readInt64(f, &(fInfo->size), &read_bytes));
  ERROR_TEST(readInt64(f, &(info->dataSize), &read_bytes));
  ERROR_TEST(readChar(f, &(info->endUnusedBits), &read_bytes));
  ERROR_TEST(readChar(f, &(info->flags), &read_bytes));
  ERROR_TEST(readInt64(f, &(info->haffTreeSize), &read_bytes));
  if (NULL == (info->haffTree = malloc(info->haffTreeSize / 8 + 1))) {
    MEMORY(L"Memory allocate error!");
    LOGGING_FUNC_STOP;
    return MEMORY_ALLOCATE_ERROR;
  }
  ERROR_TEST(readNBytes(f, info->haffTreeSize / 8, info->haffTree, &read_bytes));

  ERROR_TEST(readInt64(f, &(info->HeaderCheckSum), &read_bytes));

  LOGGING_FUNC_STOP;
  return 0;
}

#undef ERROR_TEST
