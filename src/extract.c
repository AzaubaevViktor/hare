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

int extract(FILE *f, size_t offset, ArchFileInfo *info, char *fileName) {
  LOGGING_FUNC_START;
  int _error = 0;
  FILE *fOut = NULL;
  char *buf = malloc(BUF_SIZE*sizeof(char));
  char *buf2Write = malloc(BUF_SIZE * sizeof(char));
  char charBuf = 0;
  size_t readBytes = 0;
  size_t returnBits = 0;
  size_t usedBits = 0;
  size_t readedBytes = 0;
  char pow2[8] = {0, 1, 3, 7, 15, 31, 63, 127};

  if (0 != (_error = fseek(f, offset, SEEK_SET))) {
    IO(L"`fseek` return `%d`", _error);
    LOGGING_FUNC_STOP;
    return FSEEK_ERROR;
  }

  if (NULL == (fOut = fopen(fileName, "wb"))) {
    IO(L"Couldnt open file `%s`", fileName);
    LOGGING_FUNC_STOP;
    return FILE_OPEN_ERROR;
  }

  for (readedBytes = 0; readedBytes < info->dataSize; readedBytes += readBytes) {
    _error = readNBytes(f, BUF_SIZE, buf, &readBytes);
    buf2Write = decoding(buf, BUF_SIZE*8, &returnBits);
    while (returnBits != 0) {
      /* magic */
      charBuf |= (buf2Write[BUF_SIZE - returnBits / 8] >> usedBits) & pow2[7 - usedBits];
      if (returnBits < (8 - usedBits)) {
        usedBits -= returnBits;
        returnBits = 0;
      } else {
        returnBits -= (8-usedBits);
        usedBits = 0;
        writeChar(fOut, charBuf, 0);
        charBuf = 0;
      }
    }
  }
  writeNBytes(fOut, 0, &charBuf, 1);

  close(fOut);

  return 0;
  LOGGING_FUNC_STOP;
}
