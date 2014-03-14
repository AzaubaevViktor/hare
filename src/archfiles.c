/* 2014, Korovin Viktor <sarvash.inc@gmail.com> Потоковая запись файла */
#include "archfiles.h"


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

int writeData(FILE *f, int64_t size, void *data) {
  return writeNBytes(f, size, data, 0);
}

int readHeader(FILE *f, FileInfo *file) {

}
