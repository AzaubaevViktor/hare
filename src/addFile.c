#include  "addFile.h"

int addFile2Arch(const char* archName, ArchFileInfo archFileInfo)
{
    FILE* archive;
    FILE* file;

    const size_t sizeBlock = 50;
    int countCodingBits = 0;
    size_t sizeReadBlock;
    char block[sizeBlock];
    char byteForWrite;
    char partialByte = 0;
    int `countUsedBits = 0;
    int i;

    archive = fopen(archName, "ab");
    file    = fopen(archFileInfo.fileInfo->name, "rb");

    if (!archive || !file)
    {
        return OPEN_FILE_ERROR;
    }

    while(!feof(file))
    {
        readNBytes(file, sizeBlock, block, &sizeReadBlock);

        coding(archFileInfo.haffTree, block, block, countCodingBits);

        for (i = 0; i < countCodingBits / 8; i++)
        {
            byteForWrite = 0;
            byteForWrite |= partialByte;
            byteForWrite |= (block[i] >> countUsedBits);

            partialByte = (block[i] << (8 - countUsedBits));

            writeNBytes(archive, 1, byteForWrite, 0);
        }

        if (countCodingBits % 8)
        {
            partialByte = block[countCodingBits / 8];
            countUsedBits = countCodingBits % 8;
        }
        else
        {
            partialByte = 0;
            countUsedBits = 0;
        }
    }

    fclose(archive);
    fclose(file);
    return 0;
}

void coding(char* huffTree, char* bytesForCoding, char* codingBits, int* countCodingBits)
{
    codingBits = bytesForCoding;
    *countCodingBits = strlen(bytesForCoding) * 8;
}
