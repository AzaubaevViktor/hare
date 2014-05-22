/* 2014, Korovin Viktor <sarvash.inc@gmail.com> Потоковая запись файла
 * Будьте осторожны с writeBytes! Не пытайтесь одновременно писать в два файла, поведение будет не определено
 */
#include "lowfile.h"

#define elif else if

/* ======================== WRITE ===================== */

crc _writeNBytes(FILE *f, int64_t N, char *str, int _crc_comm) {
  static crc crcTable[256];
  static crc remainder = INITIAL_REMAINDER;

  if (1 == _crc_comm) {
    crcInit(crcTable);
    remainder = INITIAL_REMAINDER;
    return 0;
  }

  if (2 == _crc_comm) {
    return remainder;
  }

  //Write
  fwrite(str, N, 1, f);
  if (ferror(f)) {
    WARNING(L"Writing error");
    return IO_WRITE_ERROR;
  }

  //CRC
  crcFast((unsigned char const *) str, N, crcTable, &remainder);

  return 0;
}


void initWrCrc() {
  _writeNBytes(NULL, 0, NULL, 1);
}


crc getWrCrc() {
  return _writeNBytes(NULL, 0, NULL, 2);
}


int writeNBytes(FILE *f, int64_t N, char *str) {
  return _writeNBytes(f, N, str, 0);
}


int writeInt64(FILE *f, int64_t num) {
  int64_t _num = num;
  char tmp[INT64SIZE] = "";
  size_t i = 0;
  for (i=0; i<INT64SIZE; i++) {
    tmp[i] = (unsigned char) (_num & 0xFF);
    _num = _num >> 8;
  }
  return writeNBytes(f, INT64SIZE, tmp);
}


int writeCrc(FILE *f, crc num) {
  crc _num = num;
  char tmp[sizeof(crc)] = "";
  size_t i = 0;
  for (i=0; i<sizeof(crc); i++) {
    tmp[i] = (unsigned char) (_num & 0xFF);
    _num = _num >> 8;
  }
  return writeNBytes(f, sizeof(crc), tmp);
}


int writeChar(FILE *f, char ch) {
  return writeNBytes(f, 1, &ch);
}


/* ====================== READ ========================= */

crc _readNBytes(FILE *f, uint64_t N, char *str, size_t *read_bytes, int _crc_comm) {
  int _error = 0;
  static crc crcTable[256];
  static crc remainder = INITIAL_REMAINDER;

  //LOGGING_FUNC_START;

  *read_bytes = 0;

  if (1 == _crc_comm) {
    crcInit(crcTable);
    remainder = INITIAL_REMAINDER;
    return 0;
  }

  if (2 == _crc_comm) {
    return remainder;
  }

  //Read

  *read_bytes = fread(str, 1, N, f);

  if (ferror(f)) {
    WARNING(L"Read error!");
    return IO_READ_ERROR;
  }

  //CRC
  crcFast((unsigned char const *) str, N, crcTable, &remainder);

  if (feof(f)) {
    _error = IO_EOF;
  }

  return _error;
}


int readNBytes(FILE *f, uint64_t N, char *str, size_t *read_bytes) {
  return _readNBytes(f, N, str, read_bytes, 0);
}


void initRdCrc() {
  size_t rd_bytes;
  _readNBytes(NULL, 0, NULL, &rd_bytes, 1);
}


crc getRdCrc() {
  size_t rd_bytes;
  return _readNBytes(NULL, 0, NULL, &rd_bytes, 2);
}


int readInt64(FILE *f, int64_t *num, size_t *read_bytes) {
  char tmp[INT64SIZE] = "";
  size_t i = 0;
  int _error = 0;
  //LOGGING_FUNC_START;
  *read_bytes = 0;
  _error = readNBytes(f, INT64SIZE, tmp, read_bytes);
  if (_error) {
    //WARNING(L"Read error `%d`", _error);
    //LOGGING_FUNC_STOP;
    return _error;
  }
  for (i=0; i<INT64SIZE; i++) {
    *num = *num << 8;
    *num += (unsigned char) tmp[INT64SIZE - i - 1];
  }
  return 0;
}


int readCrc(FILE *f, crc *num, size_t *read_bytes) {
  char tmp[sizeof(crc)] = "";
  size_t i = 0;
  int _error = 0;
  *read_bytes = 0;
  _error = readNBytes(f, sizeof(crc), tmp, read_bytes);
  if (_error) {
    return _error;
  }
  for (i=0; i<sizeof(crc); i++) {
    *num = *num << 8;
    *num += (unsigned char) tmp[sizeof(crc) - i - 1];
  }
  return 0;
}


int readChar(FILE *f, char *ch, size_t *read_bytes) {
  //IO(L"Read char");
  return readNBytes(f, 1, ch, read_bytes);
}
