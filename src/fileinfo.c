#include "fileinfo.h"


int getFileInfo(const char * fileName, struct FileInfo * fileInfo)
{
    struct stat bufferFileInfo;

    if(0 == stat(fileName, &bufferFileInfo))
    {
        fileInfo->sizeName = strlen(fileName);

        fileInfo->name = (char*)calloc(fileInfo->name, fileInfo->sizeName + 1);
        if(NULL == fileInfo->name)
            return -1;
        else
            strcpy(fileInfo->name, fileName);

        fileInfo->size                  = (int64_t)bufferFileInfo.st_size;

        fileInfo->timeLastModification  = bufferFileInfo.st_mtime;
    }
    else
        return -1;

    return 0;
}


void printFileInfo(struct FileInfo fileInfo)
{
    printf("================ File Info ==================\n"
           "Name:                   %s\n"
           "Size:                   %d\n"
           "Time last modification: %s",
           fileInfo.name,
           fileInfo.size,
           ctime(&fileInfo.timeLastModification));
}
