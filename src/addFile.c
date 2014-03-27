#include  "addFile.h"

int addFile2Arch(const char* archName, ArchFileInfo archFileInfo)
{
    FILE* archive;
    FILE* file;

    const size_t sizeBlockBytes = 50;
    size_t sizeReadBlockBytes;
    char blockBytes[sizeBlockBytes];

    archive = fopen(archName, "ab");
    file    = fopen(archFileInfo.fileInfo->name, "rb");

    if (!archive || !file)
    {
        return OPEN_FILE_ERROR;
    }

    while(!feof(file))
    {
        readNBytes(file, sizeBlockBytes, blockBytes, &sizeReadBlockBytes);
        writeNBytes(archive, sizeReadBlockBytes, coding(archFileInfo.haffTree, blockBytes), 0);
    }

    fclose(archive);
    fclose(file);
    return 0;
}

char* coding(char* huffTree, char* bytesForCoding)
{
    return bytesForCoding;
}
