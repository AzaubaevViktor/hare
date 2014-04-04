#include  "addFile.h"

int addFile2Arch(/*const char* archName, ArchFileInfo archFileInfo*/)
{
    FILE* archive;
    FILE* file;

    const size_t sizeBlock = 50;
    int countCodingBits = 0;
    size_t sizeReadBlock;
    char block[sizeBlock];
    char byteForWrite;
    char partialByte = 0;
    int countUsedBits = 0;
    int i;
    char left1[9] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF};
    char right1[9] = {0, 1, 3, 7, 15, 31, 63, 127, 255};
/*
    archive = fopen(archName, "ab");
    file    = fopen(archFileInfo.fileInfo->name, "rb");
*/
    archive = fopen("lalka.txt", "ab");
    file    = fopen("file.txt", "rb");

    if (!archive || !file)
    {
        return OPEN_FILE_ERROR;
    }

    while(!feof(file))
    {
        readNBytes(file, sizeBlock, block, &sizeReadBlock);

        //coding(archFileInfo.haffTree, block, block, countCodingBits);
        coding(block, block, block, countCodingBits);

        for (i = 0; i < countCodingBits / 8; i++)
        {
            byteForWrite = 0;
            byteForWrite |= partialByte;
            byteForWrite |= ((block[i] >> countUsedBits) & right1[8 - countUsedBits]);

            partialByte = (block[i] << (8 - countUsedBits)) & left1[countUsedBits];

            writeNBytes(archive, 1, byteForWrite, 0);
        }
        
        if (countCodingBits % 8)
        {
            if ((countUsedBits + countCodingBits % 8) >= 8)
            {
            	byteForWrite = 0;
                byteForWrite |= partialByte;
            	byteForWrite |= ((block[countCodingBits / 8] >> countUsedBits) & right1[8 - countUsedBits]);

            	partialByte = (block[countCodingBits / 8] << (8 - countUsedBits)) & left1[countUsedBits];

	            writeNBytes(archive, 1, byteForWrite, 0);
            }
            else
            {
                partialByte |= ((block[countCodingBits / 8] >> countUsedBits) & right1[8 - countUsedBits]);
                countUsedBits += countCodingBits % 8;
            }
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
