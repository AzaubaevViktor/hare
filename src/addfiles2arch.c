#include "addfiles2arch.h"

int writeBytes(FILE *f, unsigned char *str, int drop) {
  static unsigned char buf[BUF_LEN];
  static uint64_t pos;
  int64_t ext_pos = 0;

  IO("Add string to buffer")

  while (str[ext_pos]) {
    /* TODO: сделать нормульную копирование строк через strcpy */
    buf[pos] = str[ext_pos];
    if (++pos == BUF_LEN) {
      IO("Drop buffer")
      pos = 0;
      _writeBytes(f, buf, BUF_LEN);
    }
    ext_pos++;
  }

  if (drop) {
    IO("Forsed drop buffer")
    _writeBytes(f, buf, pos);
  }

}

int _writeBytes(FILE *f, unsigned char *buf, size_t k_bytes) {
  size_t wr_bytes = 0;
  IO("Write bytes on file, bs=%d", BUF_LEN)
  wr_bytes = fwrite(buf, k_bytes, 1, f);
  IO("Wrote %zdx%zd bytes", wr_bytes, k_bytes)
  if (k_bytes & (!wr_bytes)) {
    WARNING("Writing error")
    return IO_WRITE_ERROR;
  }
  return 0;
}


int generateFileHeader(FILE *f) {
  writeBytes(f, "00", 0);
  return 0;
}
