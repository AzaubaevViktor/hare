#include "listfiles.h"

int printFilesOfFolder(FILE *arch, char *nameFolder)
{
    char *currentNameFile;
    ArchFileInfo **foldersArch = (ArchFileInfo **)calloc(1, sizeof(ArchFileInfo *)),
                 **filesArch   = (ArchFileInfo **)calloc(1, sizeof(ArchFileInfo *));
    ArchFileInfo *finfo = (ArchFileInfo *)malloc(sizeof(ArchFileInfo));
    int64_t howFolders = 0, howFiles = 0;
    int64_t blocksFolder = 1, blocksFile = 1;
    int err;
    char *nameFolderCan = pathToCanon(nameFolder);
    LOGGING_FUNC_START;

    while ((err = readHeader(arch, finfo)) != SIGNATURE_ERROR){
        if (err != 0){
            ERROR(L"Some problem in readHeader with code %d", err);
            LOGGING_FUNC_STOP;
            return READ_ARCH_ERROR;
        }
        currentNameFile = finfo->fileInfo->name;
        if (isFolder(currentNameFile) && !strncmp(currentNameFile, nameFolderCan, strlen(nameFolderCan))) {
            howFolders++;
            if (howFolders > blocksFolder * SIZE_BLOCK){
                blocksFolder++;
                if ((foldersArch = (ArchFileInfo **)realloc(foldersArch, blocksFolder * SIZE_BLOCK * sizeof(ArchFileInfo *))) == NULL){
                    MEMORY(L"Memory allocate error!");
                    LOGGING_FUNC_STOP;
                    return MEMORY_ALLOCATE_ERROR;
                }
            }

            foldersArch[howFolders - 1] = finfo;
        } else if (!isFolder(currentNameFile) && !strncmp(currentNameFile, nameFolderCan, strlen(nameFolderCan))){
            howFiles++;
            if (howFiles > blocksFile * SIZE_BLOCK){
                blocksFile++;
                if ((filesArch = (ArchFileInfo **)realloc(filesArch, blocksFile * SIZE_BLOCK * sizeof(ArchFileInfo *))) == NULL){
                    MEMORY(L"Memory allocate error!");
                    LOGGING_FUNC_STOP;
                    return MEMORY_ALLOCATE_ERROR;
                }
            }

            filesArch[howFiles - 1] = finfo;
        }
    }

    currentNameFile = finfo->fileInfo->name;



    LOGGING_FUNC_STOP;
    return 0;
}
