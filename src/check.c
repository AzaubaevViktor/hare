#include "check.h"
#include "path.h"
int checkIntegrity_my(FILE *arch){
    ArchFileInfo *info = (ArchFileInfo *)malloc(sizeof(ArchFileInfo));
    info->fileInfo = (FileInfo *)malloc(sizeof(FileInfo));
    crc realsum, shouldsum;
    initRdCrc();
    int err = 0;

    while ((err = checkingHeader(arch, info) != IO_EOF)){
        if (isFolder(info->fileInfo->name)) continue;
        printf("%s\n", info->fileInfo->name);
        printf("Header: ");
        if (err == 0) printf("OK\n");
        else if (err == 1) printf("BAD %lx\n",  info->HeaderCheckSum);
        err = checkingData(arch, info, &realsum, &shouldsum);
        printf("Data  : ");
        if (err == 0) printf("OK\n"); else
            if (err == 1) printf("BAD %lx %lx\n", realsum, shouldsum);

    }
    free(info->fileInfo);
    free(info);
    return 0;
}
int checkingHeader(FILE *arch, ArchFileInfo *info){
    initRdCrc();
    int err = readHeader(arch, info);
    if (feof(arch)) return IO_EOF;
    if (err == SIGNATURE_ERROR){
        printf("Check Error on %lx\n", ftell(arch));
        findSignature(arch);
        if (feof(arch)) return IO_EOF;
        printf("Find on %lx\n", ftell(arch));
        return checkingHeader(arch, info);
    }
    if (err == HASH_HEADER_CHECK_ERROR) return 1; else return 0;
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
