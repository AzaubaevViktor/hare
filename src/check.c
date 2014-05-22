#include "check.h"

int checkIntegrity_my(FILE *arch){
    ArchFileInfo *info = (ArchFileInfo *)malloc(sizeof(ArchFileInfo));
    info->fileInfo = (FileInfo *)malloc(sizeof(FileInfo));
    int64_t realsum;

    initRdCrc();
    int err = 0;

    while ((err = checkingHeader(arch, info, &realsum) != IO_EOF)){
        if (err == 0) printf("OK\n");
        else if (err == 1) printf("BAD %ld %ld\n", realsum, info->HeaderCheckSum);
        err = checkingData(arch, info, &realsum);
        if (err == 0) printf("OK\n"); else
            if (err == 1) printf("BAD %ld %ld\n", realsum, info->HeaderCheckSum);
    }
    free(info->fileInfo);
    free(info);
    return 0;
}
int checkingHeader(FILE *arch, ArchFileInfo *info, int64_t *real){
    initRdCrc();
    if (feof(arch)) return IO_EOF;

    int err = readHeader(arch, info);
    if (err == SIGNATURE_ERROR){
        findSignature(arch);
        return checkingHeader(arch, info, real);
    }

    *real = getRdCrc();
    if (*real == info->HeaderCheckSum) {
        return 0;
    } else {
        return 1;
    }

}
int checkingData(FILE *arch, ArchFileInfo *info, int64_t *real){
    initRdCrc();
    size_t read_bytes;
    char *str = (char *)malloc(info->dataSize);
    readNBytes(arch, info->dataSize, str, &read_bytes);
    *real = getRdCrc();
    int64_t should;
    readInt64(arch, &should, &read_bytes);
    if (*real == should){
        return 0;
    } else {
        return 1;
    }

}
