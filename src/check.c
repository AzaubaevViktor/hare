#include "check.h"
#include "path.h"
int checkIntegrity_my(FILE *arch){
    ArchFileInfo *info = (ArchFileInfo *)malloc(sizeof(ArchFileInfo));
    info->fileInfo = (FileInfo *)malloc(sizeof(FileInfo));
    crc realsum, shouldsum;
    int i = 0;
    initRdCrc();
    int err = 0;

    while ((err = checkingHeader(arch, info) != IO_EOF)){
        if (isFolder(info->fileInfo->name)) continue;
        printf("Header: ");
        if (err == 0) printf("OK %d\n", i);
        else if (err == 1) printf("BAD %d %lx\n", i,  info->HeaderCheckSum);
        err = checkingData(arch, info, &realsum, &shouldsum);
        printf("Data  : ");
        if (err == 0) printf("OK %d\n", i); else
            if (err == 1) printf("BAD %d %lx %lx\n", i, realsum, shouldsum);
        i++;
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
        findSignature(arch);
        return checkingHeader(arch, info);
    }
    if (err == HASH_HEADER_CHECK_ERROR) return 1; else return 0;
}
int checkingData(FILE *arch, ArchFileInfo *info, crc *real, crc *should){
    initRdCrc();
    size_t read_bytes;
    //int i; char ch;
    char *str = (char *)malloc(info->dataSize);
    readNBytes(arch, info->dataSize, str, &read_bytes);
    //for (i = 0; i < info->dataSize;i++) readChar(arch, &ch, &read_bytes);
    *real = getRdCrc();
    readCrc(arch, should, &read_bytes);
    free(str);
    if (*real == *should){
        return 0;
    } else {
        return 1;
    }

}
