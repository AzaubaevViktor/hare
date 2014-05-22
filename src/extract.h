#ifndef EXTRACT_H
#define EXTRACT_H

#include "path.h"
#include "hare.h"
#include "decodeTree.h"
#include "logging.h"
#include "lowfile.h"
#include "archfiles.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>

#define FSEEK_ERROR (7145)
#define FILE_OPEN_ERROR (8155)
#define BUF_SIZE (1024)
#define ARCHIVE_ERROR (9462)

#define max(a,b) (((a) < (b)) ? (b) : (a))

/* На вход поступает байтовая строка, считанная из файла (закодированная хаффманом)
 * Там она преобразуется, задействуются не все биты (из-за плавающей величины симовла в коде Хаффмана)
 * Внутри есть буфер (static ???), который хранит символ (например, если тот не до конца присутствует в текущей строке
 * drop сбрасывает буфер
 */

int extractFiles(FILE *f, Context *cnt);

#endif // EXTRACT_H
