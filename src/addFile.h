#ifndef ADDFILE_H
#define ADDFILE_H

#include "archfiles.h"
#include "lowfile.h"
#include <stdio.h>
#include <string.h>



#define OPEN_FILE_ERROR (1234)

//int addFile(const char* nameFileFrom, const char* nameFileTo);
int addFile(ArchFileInfo archFileInfo, const char* nameArchive);

void coding(char* huffTree, char* bytesForCoding, int countBytesForCoding, char* codingBits, int* countCodingBits);

#endif // ADDFILE_H
