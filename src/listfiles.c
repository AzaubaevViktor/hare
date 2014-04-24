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
    fpos_t archPos;
    int err = 0;
    int64_t max_len = 0;
    char *nameFolderCan = pathToCanon(nameFolder);
    LOGGING_FUNC_START;
    while ((err = readHeader(arch, info)) != IO_EOF){
        if (err != 0){
            ERROR(L"Some problem in readHeader with code %d", err);
            break;
        }
        currentNameFile = info->fileInfo->name;
        if (isFolder(currentNameFile) && (levels(nameFolderCan) == levels(currentNameFile) - 1)
                && ( strncmp(currentNameFile, nameFolderCan, strlen(nameFolderCan) - 1) == 0)) {
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
        } else if (!isFolder(currentNameFile) && (levels(nameFolderCan) == levels(currentNameFile))
                   && (strncmp(currentNameFile, nameFolderCan, strlen(nameFolderCan) -1) == 0)) {
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
        fgetpos(arch, &archPos);
        fseek(arch, archPos.__pos - (BUF_LEN - getRdPos(arch)) + info->dataSize, SEEK_SET);
        dropRdBytes(arch);
        INFO(L"Position: `%d` + `%d` ", (archPos.__pos - (BUF_LEN - getRdPos(arch))), info->dataSize);
        info = malloc(sizeof(ArchFileInfo));
        info->fileInfo = malloc(sizeof(FileInfo));

    }

    double size_can; char char_size;
    for (i = 0;i < howFolders;i++)
    {

        printf("%-*s|\n", (int)max_len, getFileByPath(nameFolderCan, foldersArch[i]->fileInfo->name) + 2);
    }
    for (i = 0;i < howFiles  ;i++)
    {
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

        printf("%-*s|%7.2f%c %3d%%\n", (int)max_len, getFileByPath(nameFolderCan, filesArch[i]->fileInfo->name) + 2,
               size_can, char_size,
               (int)(filesArch[i]->dataSize * 100 / filesArch[i]->fileInfo->size));
    }

    for(i = 0;i < howFiles;i++){
        free(filesArch[i]->fileInfo);
        free(filesArch[i]);
    }
    for (i = 0;i < howFolders;i++){
        free(foldersArch[i]->fileInfo);
        free(foldersArch[i]);
    }
    LOGGING_FUNC_STOP;
    return 0;
}
