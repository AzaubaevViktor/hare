#include "check.h"
#include "path.h"
int checkIntegrity_my(FILE *arch){
    ArchFileInfo *info = (ArchFileInfo *)malloc(sizeof(ArchFileInfo));
    info->fileInfo = (FileInfo *)malloc(sizeof(FileInfo));
    crc realsum, shouldsum;
    initRdCrc();
    int err = 0;

    while (IO_EOF != (err = checkingHeader(arch, info))){
        printf("===========================\n`%s`\n", info->fileInfo->name);
        printf("Header: ");
        if (!err) {
            printf("OK\n");
            err = checkingData(arch, info, &realsum, &shouldsum);
            printf("Data  : ");
            if (!err)
                printf("OK\n");
            else
                printf("BAD `%lX` `%lX`\n", realsum, shouldsum);
        }
        else
            printf("BAD `%lX`\n",  info->HeaderCheckSum);

    }
    free(info->fileInfo);
    free(info);
    return 0;
}
int checkingHeader(FILE *arch, ArchFileInfo *info){
    initRdCrc();
    long pos = ftell(arch);
    int err = readHeader(arch, info);
    if (err){
        fseek(arch, pos+1, SEEK_SET);
        printf("Signature error, find from `%lX`\n", ftell(arch));
        findSignature(arch);
        if (feof(arch)) return IO_EOF;
        printf("Find on `%lX`\n", ftell(arch));
        return checkingHeader(arch, info);
    }
    if (feof(arch)) return IO_EOF;
    return 0;
}
int checkingData(FILE *arch, ArchFileInfo *info, crc *real, crc *should){
    initRdCrc();
    size_t read_bytes;
    char *str = (char *)malloc(info->dataSize);
    readNBytes(arch, info->dataSize, str, &read_bytes);
    *real = getRdCrc();
    readCrc(arch, should, &read_bytes);
    free(str);
    if (*real == *should){
        return 0;
    } else {
        return 1;
    }
}
