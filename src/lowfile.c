/* 2014, Korovin Viktor <sarvash.inc@gmail.com> Потоковая запись файла
 * Будьте осторожны с writeBytes! Не пытайтесь одновременно писать в два файла, поведение будет не определено
 */
#include "lowfile.h"

#define elif else if

/* ======================== WRITE ===================== */


int _writeBytes(FILE *f, char *buf, size_t k_bytes) {
  size_t wr_bytes = 0;
  wr_bytes = fwrite(buf, k_bytes, 1, f);
  IO("Wrote %zdx%zd bytes, bs=%d", wr_bytes, k_bytes, BUF_LEN);
  if (ferror(f)) {
    WARNING("Writing error");
    return IO_WRITE_ERROR;
  }
  return 0;
}

#define OLD_WRITE_

int writeNBytes(FILE *f, int64_t N, char *str, int drop) {
  static char buf[BUF_LEN];
  static uint64_t pos;
  uint64_t ext_pos = 0;
  int wr_result = 0;
#ifndef OLD_WRITE
  size_t nBufBytes = 0;
#endif

  IO("Add %"PRId64" bytes to buffer", N);
#ifdef OLD_WRITE
  while (ext_pos < N) {
    buf[pos] = str[ext_pos];
    if (++pos == BUF_LEN) {
      IO("Drop buffer")
      pos = 0;
      wr_result = _writeBytes(f, buf, BUF_LEN);
      if (wr_result)
        return wr_result;
    }
    ext_pos++;
  }
#else
  while (ext_pos < N) {
    nBufBytes = (BUF_LEN - pos) < ((uint64_t) N - ext_pos) ? (BUF_LEN - pos) : (N - ext_pos);
    memcpy(buf+pos, str+ext_pos, nBufBytes);
    ext_pos += nBufBytes;
    pos += nBufBytes;
    if (pos == BUF_LEN) {
      IO("Drop buffer")
      pos = 0;
      wr_result = _writeBytes(f, buf, BUF_LEN);
      if (wr_result)
        return wr_result;
    }
  }
#endif

  if (drop) {
    IO("Forsed drop buffer")
    wr_result = _writeBytes(f, buf, pos);
  }

  return wr_result;
}


int writeInt64(FILE *f, int64_t num, int drop) {
  int64_t _num = num;
  char tmp[8] = "";
  int i = 0;
  IO("Write int64 num")
  for (i=0; i<8; i++) {
    tmp[i] = _num & 0xFF;
    _num = _num >> 8;
  }
  return writeNBytes(f, 8, tmp, drop);
}


int writeChar(FILE *f, char ch, int drop) {
  IO("Write char")
  return writeNBytes(f, 1, &ch, drop);
}


/* ====================== READ ========================= */

int _readBytes(FILE *f, char *buf, size_t k_bytes) {
  size_t rd_bytes = 0;
  if (feof(f)) {
    INFO("End of file");
    return IO_EOF;
  }
  rd_bytes = fread(buf, k_bytes, 1, f);
  IO("Read %zdx%zd bytes, bs=%d", rd_bytes, k_bytes, BUF_LEN);
  if (ferror(f)) {
    WARNING("Read error!");
    return IO_READ_ERROR;
  }
  return 0;
}

int readNBytes(FILE *f, uint64_t N, char *str) {
  /* TODO: разобраться с посылкой EOF */
  static char buf[BUF_LEN];
  static uint64_t pos = BUF_LEN;
  uint64_t ext_pos = 0;
  size_t nBufBytes = 0;
  int r_result = 0;
  int is_eof = 0;

  while (ext_pos < N) {
    nBufBytes = (BUF_LEN - pos) < ((uint64_t) N - ext_pos) ? (BUF_LEN - pos) : (N - ext_pos);
    memcpy(str+ext_pos, buf+pos, nBufBytes);
    ext_pos += nBufBytes;
    pos += nBufBytes;
    if (pos == BUF_LEN) {
      IO("Read file to buffer")
      pos = 0;
      r_result = _readBytes(f, buf, BUF_LEN);
      if (IO_EOF == r_result)
        is_eof = 1;
      elif (r_result)
        return r_result;
    }
  }

  if (is_eof) {
    return IO_EOF;
  } else
    return r_result;
}
