#include "listfiles.h"

int printFilesOfFolder(FILE *fileArch, const char *nameFolder)
{
    char *currentNameFile;
    ArchFileInfo **foldersArch = (ArchFileInfo **)calloc(1, sizeof(ArchFileInfo *)),
                 **filesArch   = (ArchFileInfo **)calloc(1, sizeof(ArchFileInfo *));
    ArchFileInfo *finfo = (ArchFileInfo *)malloc(sizeof(ArchFileInfo));
    int err;
    char *nameFolderCan;
    LOGGING_FUNC_START;

    err = readHeader(fileArch, finfo);
    if (err != 0){
        ERROR(L"Some problem in readHeader with code %d", err);
        LOGGING_FUNC_STOP;
        return READ_ARCH_ERROR;
    }
    currentNameFile = finfo->fileInfo->name;
    if (isFolder(currentNameFile)) {
        foldersArch[0] = finfo; //Еще нужно увеличить массив указателей
    } else {
        filesArch[0] = finfo;
    }

    LOGGING_FUNC_STOP;
    return 0;
}
