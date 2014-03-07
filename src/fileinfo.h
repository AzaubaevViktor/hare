#ifndef FILEINFO_H
#define FILEINFO_H

#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


typedef struct {
    int64_t sizeName;
    char *name;
    int64_t size;
    time_t timeLastAccess;
    time_t timeLastModification;
    time_t timeLastChange;
} FileInfo;


int getFileInfo(const char *fileName, FileInfo *fileInfo);
void printFileInfo(FileInfo fileInfo);

#endif // FILEINFO_H
