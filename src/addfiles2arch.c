#include "addfiles2arch.h"

int writeBytes(FILE *f, unsigned char *buf, size_t k_bytes) {
  size_t wr_bytes = 0;
  IO("Write bytes on file, bs=%d", BUF_LEN);
  wr_bytes = fwrite(buf, k_bytes, 1, f);
  IO("Wrote %zdx%zd bytes", wr_bytes, k_bytes);
  if (wr_bytes != k_bytes) {
    return IO_WRITE_ERROR;
  }
  return 0;
}


int generateFileHeader() {
  ;
  return 0;
}
