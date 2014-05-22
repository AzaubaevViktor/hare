#include "check.h"

int checkIntegrity_my(FILE *arch){
    ArchFileInfo *info = (ArchFileInfo *)malloc(sizeof(ArchFileInfo));
    info->fileInfo = (FileInfo *)malloc(sizeof(FileInfo));
    int64_t realsum, shouldsum;
    char *str = NULL;
    size_t read_bytes;
    initRdCrc();
    int err = 0;

    while ((err = readHeader(arch, info)) != IO_EOF){
        if (err == SIGNATURE_ERROR){
            findSignature(arch);
            continue;
        }

        realsum = getRdCrc();

        if (realsum == info->HeaderCheckSum) printf("OK1\n"); else {
            printf("BAD %ld %ld\n", realsum, info->HeaderCheckSum);
            //return -1;
        }
        initRdCrc();
        str = (char *)realloc(str, info->dataSize);
        readNBytes(arch, info->dataSize, str, &read_bytes);
        realsum = getRdCrc();
        readInt64(arch, &shouldsum, &read_bytes);
        if (realsum == shouldsum) printf("OK first file\n"); else{
            printf("BAD %ld %ld\n", realsum, shouldsum);
            return -1;
        }
        initRdCrc();
    }
    free(info->fileInfo);
    free(info);
    free(str);
    return 0;
}
