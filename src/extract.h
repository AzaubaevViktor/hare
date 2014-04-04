#ifndef EXTRACT_H
#define EXTRACT_H

#include <stdlib.h>
#include <stdio.h>
#include <logging.h>
#include <lowfile.h>
#include <archfiles.h>

#define FSEEK_ERROR (7145)
#define FILE_OPEN_ERROR (8155)
#define BUF_SIZE (5)

/* На вход поступает байтовая строка, считанная из файла (закодированная хаффманом)
 * Там она преобразуется, задействуются не все биты (из-за плавающей величины симовла в коде Хаффмана)
 * Внутри есть буфер (static ???), который хранит символ (например, если тот не до конца присутствует в текущей строке
 * drop сбрасывает буфер
 */
char *decoding(char *bytes, size_t lenBits, size_t *returnBytes, int drop);
int extract(FILE *f, size_t offset, ArchFileInfo *info, char *fileName);

#endif // EXTRACT_H