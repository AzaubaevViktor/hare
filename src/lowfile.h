/* Korovin Viktor
 * writeNBytes записывает в файл N байт из str.
 * drop = 1 сливает данные из буфера в файл (использовать перед закрытием, например)
 * ACHTUNG! Если писать writeNBytes сразу в два файла (чередовать), поведение будет неопределено, поэтому писать им можно одновременно только в один файл.
 *
 * readNBytes считывает N байт из файла в str.
 * В переменной read_bytes хранится кол-во считанных байт.
 * В конце файла функция возвращает значение IO_EOF.
 */
#ifndef LOWFILE_H
#define LOWFILE_H

#include "logging.h"
#include <inttypes.h>
#include <stdio.h>

#define BUF_LEN (64)
#define BUF_LEN_BITS (BUF_LEN*8)
#define INT64SIZE (sizeof(int64_t))

#define IO_WRITE_ERROR (2451)
#define IO_READ_ERROR  (2452)
#define IO_EOF         (2453)

int writeNBytes(FILE *f, int64_t N, char *str, int drop);
int writeInt64(FILE *f, int64_t num, int drop);
int writeChar(FILE *f, char ch, int drop);

int readNBytes(FILE *f, uint64_t N, char *str, size_t *read_bytes);
int readInt64(FILE *f, int64_t *num, size_t *read_bytes);
int readChar(FILE *f, char ch, size_t *read_bytes);

#endif // LOWFILE_H
