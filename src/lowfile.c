/* 2014, Korovin Viktor <sarvash.inc@gmail.com> Потоковая запись файла
 * Будьте осторожны с writeBytes! Не пытайтесь одновременно писать в два файла, поведение будет не определено
 */
#include "lowfile.h"

#define elif else if

/* ======================== WRITE ===================== */


int _writeBytes(FILE *f, char *buf, size_t k_bytes) {
  LOGGING_FUNC_START;
  size_t wr_bytes = 0;
  wr_bytes = fwrite(buf, k_bytes, 1, f);
  IO(L"Wrote %zdx%zd bytes, bs=%d", wr_bytes, k_bytes, BUF_LEN);
  if (ferror(f)) {
    WARNING(L"Writing error");
    LOGGING_FUNC_STOP;
    return IO_WRITE_ERROR;
  }
  LOGGING_FUNC_STOP;
  return 0;
}


int _writeNBytes(FILE *f, int64_t N, char *str, int drop) {
  static char buf[BUF_LEN];
  static uint64_t pos;
  uint64_t ext_pos = 0;
  int wr_result = 0;
  size_t nBufBytes = 0;
  LOGGING_FUNC_START;


  if (drop) {
    IO(L"Forsed drop buffer");
    wr_result = _writeBytes(f, buf, pos);
    pos = 0;
  } else {
    IO(L"Add %"PRId64 L" bytes to buffer", N);
    while (ext_pos < N) {
      nBufBytes = (BUF_LEN - pos) < ((uint64_t) N - ext_pos) ? (BUF_LEN - pos) : (N - ext_pos);
      memcpy(buf+pos, str+ext_pos, nBufBytes);
      ext_pos += nBufBytes;
      pos += nBufBytes;
      if (pos == BUF_LEN) {
        IO(L"Drop buffer")
            pos = 0;
        wr_result = _writeBytes(f, buf, BUF_LEN);
        if (wr_result) {
          LOGGING_FUNC_STOP;
          return wr_result;
        }
      }
    }
  }

  LOGGING_FUNC_STOP;
  return wr_result;
}


int dropWrBytes(FILE *f) {
  return _writeNBytes(f, 0, NULL, 1);
}


int writeNBytes(FILE *f, int64_t N, char *str) {
  return _writeNBytes(f, N, str, 0);
}


int writeInt64(FILE *f, int64_t num) {
  int64_t _num = num;
  LOGGING_FUNC_START;
  char tmp[INT64SIZE] = "";
  size_t i = 0;
  IO(L"Write int64 num")
      for (i=0; i<INT64SIZE; i++) {
    tmp[i] = _num & 0xFF;
    _num = _num >> 8;
  }
  LOGGING_FUNC_STOP;
  return writeNBytes(f, INT64SIZE, tmp);
}


int writeChar(FILE *f, char ch) {
  LOGGING_FUNC_START;
  IO(L"Write char");
  LOGGING_FUNC_STOP;
  return writeNBytes(f, 1, &ch);
}


/* ====================== READ ========================= */

int _readBytes(FILE *f, char *buf, size_t k_bytes, size_t *rd_bytes) {
  LOGGING_FUNC_START;
  if (feof(f)) {
    *rd_bytes = 0;
    IO(L"End of file");
    LOGGING_FUNC_STOP;
    return IO_EOF;
  }

  *rd_bytes = fread(buf, 1, k_bytes, f);
  IO(L"Read %zdx%d bytes, bs=%d", *rd_bytes, 1, BUF_LEN);

  if (ferror(f)) {
    WARNING(L"Read error!");
    LOGGING_FUNC_STOP;
    return IO_READ_ERROR;
  }

  LOGGING_FUNC_STOP;
  return 0;
}


int _readNBytes(FILE *f, uint64_t N, char *str, size_t *read_bytes, int drop) {
  static char buf[BUF_LEN];
  static uint64_t pos = 0;
  static size_t rd_bytes = 0;
  static int is_eof = 0;
  size_t i = 0;
  uint64_t ext_pos = 0;
  size_t nBufBytes = 0;
  int r_result = 0;

  LOGGING_FUNC_START;

  *read_bytes = 0;

  if (drop) {
    IO(L"Drop read buffer");
    pos = 0;
    rd_bytes = 0;
    is_eof = 0;
  } else {
    IO(L"Read %"PRId64 L" bytes", N);
    while (ext_pos < N) {
      nBufBytes = (rd_bytes - pos) < (N - ext_pos) ? (rd_bytes - pos) : (N - ext_pos);
      memcpy(str+ext_pos, buf+pos, nBufBytes);
      ext_pos += nBufBytes;
      pos += nBufBytes;
      *read_bytes += nBufBytes;

      if (pos >= rd_bytes) {
        if (is_eof) {
          for (i=ext_pos-1; i<N; i++) {
            str[i] = '\0';
          }
          LOGGING_FUNC_STOP;
          return IO_EOF;
        }

        IO(L"Read file to buffer");
        pos = 0;
        r_result = _readBytes(f, buf, BUF_LEN, &rd_bytes);

        is_eof = (r_result == IO_EOF) ? 1 : 0;

        if ((r_result) && (IO_EOF != r_result)) {
          LOGGING_FUNC_STOP;
          return r_result;
        }
      }
    }
  }

  LOGGING_FUNC_STOP;
  return r_result;
}


int dropRdBytes(FILE *f) {
  size_t read_bytes = 0;
  return _readNBytes(f, 0, NULL, &read_bytes, 1);
}


int readNBytes(FILE *f, uint64_t N, char *str, size_t *read_bytes) {
  return _readNBytes(f, N, str, read_bytes, 0);
}


int readInt64(FILE *f, int64_t *num, size_t *read_bytes) {
  char tmp[INT64SIZE] = "";
  size_t i = 0;
  LOGGING_FUNC_START;
  *read_bytes = 0;
  readNBytes(f, INT64SIZE, tmp, read_bytes);
  if (*read_bytes < INT64SIZE) {
    WARNING(L"Read error!");
    LOGGING_FUNC_STOP;
    return IO_READ_ERROR;
  }
  for (i=0; i<INT64SIZE; i++) {
    *num = *num << 8;
    *num += tmp[INT64SIZE - i - 1];
  }
  LOGGING_FUNC_STOP;
  return 0;
}


int readChar(FILE *f, char ch, size_t *read_bytes) {
  LOGGING_FUNC_START;
  IO(L"Read char");
  readNBytes(f, 1, &ch, read_bytes);
  LOGGING_FUNC_STOP;
  return 0;
}
