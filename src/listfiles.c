#include "listfiles.h"

int printFilesOfFolder(FILE *fileArch, const char *nameFolder)
{
    LOGGING_FUNC_START
    ArchFileInfo *finfo = (ArchFileInfo *)malloc(sizeof(ArchFileInfo));
    int err = readHeader(fileArch, finfo);
    if (err != 0){
        ERROR(L"Some problem in readHeader with code %d", err);
        LOGGING_FUNC_STOP;
        return READ_ARCH_ERROR;
    }


    LOGGING_FUNC_STOP;
    return 0;
}
