#ifndef ADDFILE_H
#define ADDFILE_H

#include "archfiles.h"
#include "lowfile.h"
#include <stdio.h>

#define OPEN_FILE_ERROR (1234)

/*
 * функция, которая записывает файл в архив
 *      archName     - имя архива
 *      archFileInfo - заголовок файла, который надо записать
 *                     в заголовке уже хранится имя файла, который надо закодировать
 * возвращает 0 в случае успешной записи
 * иначе код ошибки
 */
int addFile2Arch(/*const char* archName, ArchFileInfo archFileInfo*/);


/*
 * функция, которая кодирует байты
 *      huffTree        - дерево Хаффмана, согласно которому кодируются байты
 *      bytesForCoding  - байты, которые кодируются
 *      codingBits      - закодированные биты
 *      countCodingBits - количество закодированных бит
 *
 * в countCodingBits записывается количество закодированных бит
 */
void coding(char* huffTree, char* bytesForCoding, char* codingBits, int* countCodingBits);

#endif // ADDFILE_H
