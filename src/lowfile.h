/* Korovin Viktor
 * writeNBytes записывает в файл N байт из str.
 * drop = 1 сливает данные из буфера в файл (использовать перед закрытием, например)
 * ACHTUNG! Если писать writeNBytes сразу в два файла (чередовать), поведение будет неопределено, поэтому писать им можно одновременно только в один файл.
 * Если возникает необходимость писать в два файла, то делать это нужно так:
 * ...
 * writeNBytes(f1, 10, str1, 0); // Пишем первую порцию данных
 * writeNBytes(f1, 5, str2, 1); // Пишем вторую порцию данных и заставляем записать изменения в файл (одновременно очищаем буфер)
 * writeNBytes(f2, 16, str3, 0); // Пишем первую порцию данных во второй файл
 * ...
 *
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

#define IO_WRITE_ERROR (2451)
#define IO_READ_ERROR  (2452)
#define IO_EOF         (2453)

int writeNBytes(FILE *f, int64_t N, char *str, int drop);
int writeInt64(FILE *f, int64_t num, int drop);
int writeChar(FILE *f, char ch, int drop);

int readNBytes(FILE *f, uint64_t N, char *str, size_t *read_bytes);

#endif // LOWFILE_H
