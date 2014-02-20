#ifndef FILEINFO_H
#define FILEINFO_H

#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


struct FileInfo
{
    int64_t sizeName;
    char * name;
    int64_t size;
    time_t timeLastAccess;
    time_t timeLastModification;
    time_t timeLastChange;
};


int getFileInfo(const char * fileName, struct FileInfo * fileInfo);
void printFileInfo(struct FileInfo fileInfo);

#endif // FILEINFO_H
