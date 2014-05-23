#include "fileinfo.h"

int getFileInfo(const char *fileName, FileInfo *fileInfo)
{
    struct stat bufferFileInfo;

    if(0 == stat(fileName, &bufferFileInfo))
    {
        fileInfo->sizeName = strlen(fileName);

        fileInfo->name = (char *)calloc(fileInfo->sizeName + 1, sizeof(char));
        if(NULL == fileInfo->name)
            return ERROR_NOT_ALLOCATED_MEMORY;
        else
            strcpy(fileInfo->name, fileName);

        fileInfo->size                  = (int64_t) bufferFileInfo.st_size;

        fileInfo->timeLastModification  = (int64_t)  bufferFileInfo.st_mtime;
    }
    else
        return ERROR_GET_FILE_INFO;
    return 0;
}


void printFileInfo(FileInfo fileInfo)
{
    printf("================ File Info ==================\n"
           "Name:                   %s\n"
           "Size:                   %d\n"
           "Time last modification: %s",
           fileInfo.name,
           fileInfo.size,
           ctime(&fileInfo.timeLastModification));
}
