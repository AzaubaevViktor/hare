#include <extract.h>

/* Декордирование
 * Принимает:
 *   bytes : Байтовую строку
 *   lenBit: Длинну в битах
 * Возвращает:
 *   returnBits : кол-во записанных бит
 */

#define min(a,b) (((a) < (b)) ? (a) : (b))

char *decoding(char *bytes, size_t lenBit, size_t *returnBits) {
  *returnBits = lenBit;
  return bytes;
}

int extract(FILE *f, size_t offset, char *fileName) {
  LOGGING_FUNC_START;
  int _error = 0;
  ArchFileInfo *info = 0;
  FILE *fOut = NULL;
  char buf[BUF_SIZE] = "";
  char bufWrite[BUF_SIZE*10] = "";
  char charBuf = 0;
  size_t readBytes = 0;
  size_t returnBits = 0;
  size_t usedBits = 0;
  char pow2[8] = {0, 1, 3, 7, 15, 31, 63, 127};

  if (0 != (_error = fseek(f, offset, SEEK_SET))) {
    ERROR(L"`fseek` return `%d`", _error);
    LOGGING_FUNC_STOP;
    return FSEEK_ERROR;
  }

  if (NULL == (fOut = fopen(fileName, "wb"))) {
    ERROR(L"Couldnt open file `%s`", fileName);
    LOGGING_FUNC_STOP;
    return FILE_OPEN_ERROR;
  }

  if (NULL == (info = calloc(1, sizeof(ArchFileInfo)))) {
    MEMORY(L"Error to allocate memory");
    LOGGING_FUNC_STOP;
    return MEMORY_ALLOCATE_ERROR;
  }

  if (NULL == (info->fileInfo = calloc(1, sizeof(FileInfo)))) {
    MEMORY(L"Error to allocate memory");
    LOGGING_FUNC_STOP;
    return MEMORY_ALLOCATE_ERROR;
  }

  if (0 != (_error = readHeader(f, info))) {
    ERROR(L"readHeader stop with errorcode `%d`", _error);
    LOGGING_FUNC_STOP;
    return _error;
  }

  do {
    _error = readNBytes(f, BUF_SIZE, buf, &readBytes);
    bufWrite = decoding(buf, BUF_SIZE*8, &returnBits);
    while (returnBits != 0) {
      /* magic */
      charBuf |= (buf[returnBits / 8] >> usedBits) & pow2[8 - usedBits];
      if (returnBits < (8 - usedBits)) {
        usedBits -= returnBits;
        returnBits = 0;
      } else {
        returnBits -= (8-usedBits);
        usedBits = 0;
      }
    }
  } while (IO_EOF != _error);


  if (-1 == (_error = readHeader(f, info))) {
    ERROR(L"`readHeader` return `%d`", _error);
    LOGGING_FUNC_STOP;
    return FSEEK_ERROR;
  }

  return 0;
  LOGGING_FUNC_STOP;
}
