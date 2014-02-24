#include "addfiles2arch.h"
/* 2014, Korovin Viktor <sarvash.inc@gmail.com> Потоковая запись файла
 * Будьте осторожны с writeBytes! Не пытайтесь одновременно писать в два файла, поведение будет не определено
 */


int _writeBytes(FILE *f, char *buf, size_t k_bytes) {
  size_t wr_bytes = 0;
  wr_bytes = fwrite(buf, k_bytes, 1, f);
  IO("Wrote %zdx%zd bytes, bs=%d", wr_bytes, k_bytes, BUF_LEN)
  if (k_bytes & (!wr_bytes)) {
    WARNING("Writing error")
    return IO_WRITE_ERROR;
  }
  return 0;
}


int writeNBytes(FILE *f, int64_t N, char *str, int drop) {
  static char buf[BUF_LEN];
  static uint64_t pos;
  int64_t ext_pos = 0;
  int wr_result = 0;

  IO("Add %"PRId64" bytes to buffer", N)

  while (ext_pos < N) {
    /* TODO: сделать нормульную копирование строк через strcpy */
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

int writeFileHeader(FILE *f, \
                    FileInfo *file, \
                    int64_t archSize, \
                    char endUnusedBits, \
                    char flags, \
                    int64_t haffTreeSize, \
                    char *haffTree, \
                    int64_t HeaderCheckSum,\
                    int drop) {
  IO("Write file header")
  writeNBytes(f, SIGNATURE_LEN, SIGNATURE, 0);
  writeInt64(f, file->sizeName, 0);
  writeNBytes(f, file->sizeName, file->name, 0);
  writeInt64(f, file->timeLastModification, 0);
  writeInt64(f, file->size, 0);
  writeInt64(f, archSize, 0);
  writeChar(f, endUnusedBits, 0);
  writeChar(f, flags, 0);
  writeInt64(f, haffTreeSize, 0);
  writeNBytes(f, haffTreeSize, haffTree, 0);
  writeInt64(f, HeaderCheckSum, drop);

  return 0;
}
