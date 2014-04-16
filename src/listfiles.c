#include "listfiles.h"

int printFilesOfFolder(FILE *arch, char *nameFolder)
{
    char *currentNameFile;
    ArchFileInfo **foldersArch = (ArchFileInfo **)calloc(1, sizeof(ArchFileInfo *)),
                 **filesArch   = (ArchFileInfo **)calloc(1, sizeof(ArchFileInfo *));
    ArchFileInfo *info = (ArchFileInfo *)malloc(sizeof(ArchFileInfo));
    info->fileInfo = (FileInfo *)malloc(sizeof(FileInfo));
    int64_t howFolders = 0, howFiles = 0;
    int64_t blocksFolder = 1, blocksFile = 1;
    fpos_t archPos;
    int err;
    char *nameFolderCan = pathToCanon(nameFolder);
    LOGGING_FUNC_START;
    while ((err = readHeader(arch, info)) != SIGNATURE_ERROR){
        if (err != 0){
            ERROR(L"Some problem in readHeader with code %d", err);
            LOGGING_FUNC_STOP;

            return READ_ARCH_ERROR;
        }
        currentNameFile = info->fileInfo->name;
        if (isFolder(currentNameFile)) {
            howFolders++;
            if (howFolders > blocksFolder * SIZE_BLOCK){
                blocksFolder++;
                if ((foldersArch = (ArchFileInfo **)realloc(foldersArch, blocksFolder * SIZE_BLOCK * sizeof(ArchFileInfo *))) == NULL){
                    MEMORY(L"Memory allocate error!");
                    LOGGING_FUNC_STOP;
                    return MEMORY_ALLOCATE_ERROR;
                }
            }

            foldersArch[howFolders - 1] = info;
        } else if (!isFolder(currentNameFile)){
            howFiles++;
            if (howFiles > blocksFile * SIZE_BLOCK){
                blocksFile++;
                if ((filesArch = (ArchFileInfo **)realloc(filesArch, blocksFile * SIZE_BLOCK * sizeof(ArchFileInfo *))) == NULL){
                    MEMORY(L"Memory allocate error!");
                    LOGGING_FUNC_STOP;
                    return MEMORY_ALLOCATE_ERROR;
                }
            }

            filesArch[howFiles - 1] = info;
        }
        fgetpos(arch, &archPos);
        fseek(arch, archPos.__pos - (BUF_LEN - getRdPos(arch)) + info->dataSize, SEEK_SET);
        dropRdBytes(arch);
        INFO(L"Position: `%d` + `%d` ", (archPos.__pos - (BUF_LEN - getRdPos(arch))), info->dataSize);
        info = malloc(sizeof(ArchFileInfo));
        info->fileInfo = malloc(sizeof(FileInfo));

    }
    currentNameFile = info->fileInfo->name;
    printf("func: %s\n", foldersArch[0]->fileInfo->name);


    LOGGING_FUNC_STOP;
    return 0;
}
