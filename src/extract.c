#include <extract.h>

/* Декордирование
 * Принимает:
 *   bytes : Байтовую строку
 *   lenBit: Длинну в битах
 * Возвращает:
 *   returnBits : кол-во записанных бит
 */

#define min(a,b) (((a) < (b)) ? (a) : (b))


char *decoding(char *bytes, size_t lenBits, size_t *returnBytes, int drop) {
  *returnBytes = lenBits / 8;
  return bytes;
}


int extract(FILE *f, ArchFileInfo *info, char *fileName) {
  LOGGING_FUNC_START;
  int _error = 0;
  FILE *fOut = NULL;
  char *buf = malloc(BUF_SIZE*sizeof(char));
  char *buf2Write = NULL;
  int dropBuf = 0;
  size_t lenBits = 0;
  size_t readBytes = 0;
  size_t returnBytes = 0;
  size_t readedBytes = 0;
  size_t howManyBytesRead = 0;

  if (NULL == (fOut = fopen(fileName, "wb"))) {
    IO(L"Couldnt open file `%s`", fileName);
    LOGGING_FUNC_STOP;
    return FILE_OPEN_ERROR;
  }

  dropWrBytes(fOut);

  for (readedBytes=0; readedBytes < info->dataSize;) {
    howManyBytesRead = min(BUF_SIZE, (info->dataSize - readedBytes));
    _error = readNBytes(f, howManyBytesRead, buf, &readBytes);

    readedBytes += readBytes;

    if ((howManyBytesRead == BUF_SIZE) & (IO_EOF ==  _error)) {
      LOGGING_FUNC_STOP;
      IO(L"Error reading archive file");
      return IO_READ_ERROR;
    }

    lenBits = (howManyBytesRead < BUF_SIZE)
              ? ((dropBuf = 1), readBytes*8 - info->endUnusedBits)
              : ((dropBuf = 0), readBytes*8);

    buf2Write = decoding(buf, lenBits, &returnBytes, dropBuf);

    writeNBytes(fOut, returnBytes, buf2Write);
  }

  dropWrBytes(fOut);

  fclose(fOut);

  return _error;
  LOGGING_FUNC_STOP;
}


int extractFiles(FILE *f, Context *cnt) {
  char **files = cnt->workFiles;
  char *res = NULL;
  char *currentFile = NULL;
  int64_t len = 0;
  int64_t i = 0;
  ArchFileInfo aFileInfo;
  FileInfo fInfo;
  int shifted = 0;
  int _error = 0;
  fpos_t archPos;
  LOGGING_FUNC_START;
  for (len=0; *(files+len); len++) {
    res = *(files + len);
    *(files + len) = pathToCanon(res);
  }

  INFO(L"Len:%d", len);
  aFileInfo.fileInfo = &fInfo;

  while (IO_EOF != _error) {
    _error = readHeader(f, &aFileInfo);

    if (_error) {
      WARNING(L"readHeader return `%d` error", _error);
      LOGGING_FUNC_STOP;
      return _error;
    }

    INFO(L"File: %s", aFileInfo.fileInfo->name);

    shifted = 0;

    for (i=0; i<len; i++) {
      if ((*(files + i)) &&
          (pathInDest(*(files + i), aFileInfo.fileInfo->name))) {
        currentFile = getFileByPath(*(files + i), aFileInfo.fileInfo->name);
        INFO(L"Current file: `%s`", currentFile);
        if (isFolder(*(files + i))) {
          INFO(L"Folder");
          ;
        } else {
          IO(L"Extract `%s` to `%s`", aFileInfo.fileInfo->name, *(files + i));
          _error = extract(f, &aFileInfo, *(files + i));
          shifted = 1;
        }
        free(*(files+i));
        free(currentFile);
        currentFile = NULL;
      }
    }

    if (!shifted) {
      fgetpos(f, &archPos);
      fseek(f, archPos.__pos - (BUF_LEN - getRdPos(f)) + aFileInfo.dataSize, SEEK_SET);
      dropRdBytes(f);
      INFO(L"Position: `%d` + `%d` ", (archPos.__pos - (BUF_LEN - getRdPos(f))), aFileInfo.dataSize);
    }
  }

  LOGGING_FUNC_STOP;
  return 0;
}
