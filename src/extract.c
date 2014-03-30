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
  *returnBits = lenBit - 1;
  return bytes;
}


char _getBitByOffset(unsigned char *str, size_t offset) {
  int byte = offset / 8;
  int bit = offset - byte * 8;
  unsigned char left[9] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF};
  unsigned char right[9] = {0, 1, 3, 7, 15, 31, 63, 127, 255};
  return ((str[byte] << (bit)) & left[8 - bit]) | ((str[byte+1] >> bit) & right[bit]);
}


int extract(FILE *f, size_t offset, ArchFileInfo *info, char *fileName) {
  LOGGING_FUNC_START;
  int _error = 0;
  FILE *fOut = NULL;
  char *buf = malloc(BUF_SIZE*sizeof(char));
  char *buf2Write = NULL;
  char charBuf = 0;
  size_t readBytes = 0;
  size_t returnBits = 0;
  size_t writedBits = 0;
  size_t posBuf = 0;
  char usedBits = 0;
  size_t readedBytes = 0;
  size_t howManyBytesRead = 0;
  char left1[9] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF};
  char right1[9] = {0, 1, 3, 7, 15, 31, 63, 127, 255};

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

  for (readedBytes=0; readedBytes < info->dataSize;) {
    howManyBytesRead = min(BUF_SIZE, (info->dataSize - readedBytes));
    _error = readNBytes(f, howManyBytesRead, buf, &readBytes);
    readedBytes += readBytes;

    buf2Write = decoding(buf, readBytes*8, &returnBits);

    posBuf = 0;

    while (returnBits > posBuf) {
      writedBits = min(returnBits - posBuf, 8 - usedBits);
      charBuf |= (_getBitByOffset(buf2Write, posBuf) >> usedBits) & left1[usedBits + writedBits] & right1[8 - usedBits];


      posBuf += writedBits;
      usedBits += writedBits;

      if (8 == usedBits) {
        writeChar(fOut, charBuf);
        charBuf = 0;
        usedBits = 0;
      }

    }

  }

  writeNBytes(fOut, 0, &charBuf);
  dropWrBytes(fOut);

  close(fOut);

  return 0;
  LOGGING_FUNC_STOP;
}
