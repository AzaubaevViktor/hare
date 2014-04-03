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
  bytes[lenBits/16] = 'F';
  return bytes;
}


int extract(FILE *f, size_t offset, ArchFileInfo *info, char *fileName) {
  LOGGING_FUNC_START;
  int _error = 0;
  FILE *fOut = NULL;
  char *buf = malloc(BUF_SIZE*sizeof(char));
  char *buf2Write = NULL;
  char charBuf = 0;
  int dropBuf = 0;
  size_t lenBits = 0;
  size_t readBytes = 0;
  size_t returnBytes = 0;
  size_t readedBytes = 0;
  size_t howManyBytesRead = 0;

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

  dropRdBytes(f);
  dropWrBytes(fOut);

  for (readedBytes=0; readedBytes < info->dataSize;) {
    howManyBytesRead = min(BUF_SIZE, (info->dataSize - readedBytes));
    _error = readNBytes(f, howManyBytesRead, buf, &readBytes);

    readedBytes += readBytes;

    if ((howManyBytesRead == BUF_SIZE) & (IO_EOF ==  _error)) {
      LOGGING_FUNC_STOP;
      IO(L"Error reading archive file by offset `%zd`", offset);
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

  return 0;
  LOGGING_FUNC_STOP;
}
