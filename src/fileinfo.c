#include "fileinfo.h"

int getFileInfo(const char *fileName, FileInfo *fileInfo)
{
    struct stat bufferFileInfo;

    if(0 == stat(fileName, &bufferFileInfo))
    {
        fileInfo->sizeName = strlen(fileName);

        fileInfo->name = (char *) calloc(fileInfo->sizeName + 1, sizeof(char));
        if(NULL == fileInfo->name)
            return -1;
        else
            strcpy(fileInfo->name, fileName);

        fileInfo->size                  = (int64_t) bufferFileInfo.st_size;

        fileInfo->timeLastAccess        = bufferFileInfo.st_atime;
        fileInfo->timeLastChange        = bufferFileInfo.st_ctime;
        fileInfo->timeLastModification  = bufferFileInfo.st_mtime;
    }
    else
        return -1;

    return 0;
}


void printFileInfo(FileInfo fileInfo)
{
    printf("================ File Info ==================\n"
           "Name:                   %s\n"
           "Size:                   %d\n"
           "Time last access:       %s"
           "Time last modification: %s"
           "Time last change:       %s",
           fileInfo.name,
           fileInfo.size,
           ctime(&fileInfo.timeLastAccess),
           ctime(&fileInfo.timeLastModification),
           ctime(&fileInfo.timeLastChange));
}
