#include "listfiles.h"

int printFilesOfFolder(FILE *arch, char *nameFolder)
{
    char *currentNameFile;
    ArchFileInfo **foldersArch = NULL,
                 **filesArch   = NULL;
    ArchFileInfo *info = (ArchFileInfo *)malloc(sizeof(ArchFileInfo));
    info->fileInfo = (FileInfo *)malloc(sizeof(FileInfo));
    int64_t howFolders = 0, howFiles = 0;
    int64_t blocksFolder = 0, blocksFile = 0;
    int64_t i;

    int err = 0;
    char *canCurrentNameFile = NULL;
    int64_t max_len = 0;
    char *nameFolderCan = pathToCanon(nameFolder);
    LOGGING_FUNC_START;
    while ((err = readHeader(arch, info)) != IO_EOF){
        //printf("%lx\n", ftell(arch));
        if (err != 0){
            findSignature(arch);
            //printf("%lx\n", ftell(arch));
            if (feof(arch)) break;
            else continue;
        }
        currentNameFile = info->fileInfo->name;
        canCurrentNameFile = pathToCanon(currentNameFile);
        if (isFolder(canCurrentNameFile) && (levels(nameFolderCan) == levels(canCurrentNameFile) - 1)
                && ( strncmp(canCurrentNameFile, nameFolderCan, strlen(nameFolderCan) - 1) == 0)) {
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
            if ((int64_t)strlen(currentNameFile) > max_len) {
                max_len = strlen(currentNameFile);
            }
        } else if (!isFolder(canCurrentNameFile) && (levels(nameFolderCan) == levels(canCurrentNameFile))
                   && (strncmp(canCurrentNameFile, nameFolderCan, strlen(nameFolderCan) -1) == 0)) {
            howFiles++;
            //printf("DEBUG: %s | %s \n", currentNameFile, )
            if (howFiles > blocksFile * SIZE_BLOCK){
                blocksFile++;
                if ((filesArch = (ArchFileInfo **)realloc(filesArch, blocksFile * SIZE_BLOCK * sizeof(ArchFileInfo *))) == NULL){
                    MEMORY(L"Memory allocate error!");
                    LOGGING_FUNC_STOP;
                    return MEMORY_ALLOCATE_ERROR;
                }
            }

            filesArch[howFiles - 1] = info;
            if ((int64_t)strlen(currentNameFile) > max_len){
                max_len = strlen(currentNameFile);
            }
        }

        info = malloc(sizeof(ArchFileInfo));
        info->fileInfo = malloc(sizeof(FileInfo));
        free(canCurrentNameFile);
    }

    double size_can; char char_size;
    for (i = 0;i < howFolders;i++)
    {
        canCurrentNameFile = pathToCanon(foldersArch[i]->fileInfo->name);
        /*if (strchr(canCurrentNameFile, '/') - canCurrentNameFile + 1
                != strlen(canCurrentNameFile) - 1){
            continue;
        }*/
        printf("%-*s|\n", (int)max_len, getFileByPath(nameFolderCan, foldersArch[i]->fileInfo->name) + 2);
        free(canCurrentNameFile);
    }
    for (i = 0;i < howFiles  ;i++)
    {
        canCurrentNameFile = pathToCanon(filesArch[i]->fileInfo->name);
        /*if ( strchr(canCurrentNameFile + 2,'/' ) != NULL){
            continue;
        }*/
        size_can = (double)filesArch[i]->fileInfo->size;
        char_size = 'b';
        if (size_can > 1024){
            size_can /= 1024;
            char_size = 'K';
            if (size_can > 1024){
                size_can /= 1024;
                char_size = 'M';
            }
        }

        printf("%-*s|%7.2f%c \n",
               (int)max_len,
               getFileByPath(nameFolderCan, canCurrentNameFile) + 2,
               size_can, char_size
               );
        free(canCurrentNameFile);
    }

    for(i = 0;i < howFiles;i++){
        free(filesArch[i]->fileInfo);
        free(filesArch[i]);
    }
    for (i = 0;i < howFolders;i++){
        free(foldersArch[i]->fileInfo);
        free(foldersArch[i]);
    }
    free(info->fileInfo);
    free(info);
    free(nameFolderCan);
    LOGGING_FUNC_STOP;
    return 0;
}
