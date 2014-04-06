#include  "addFile.h"

#define DEBUG_

int addFile(ArchFileInfo archFileInfo, const char* nameArchive)
{
    FILE* archive;
    FILE* file;

    const int sizeBlock = 30000;
    int sizeReadBlock;
    char block[sizeBlock];

    int countCodingBits = 0;

    char byteForWrite;

    char countUsedBits = 0;
    char partialByte = 0;

    int i;

    char left1[9] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF};
    char right1[9] = {0, 1, 3, 7, 15, 31, 63, 127, 255};


    strcpy(block, "");

    file     = fopen(archFileInfo.fileInfo->name,  "rb");
    archive  = fopen(nameArchive,                  "ab");

    if (!archive || !file)
    {
        //printf("> ERROR OPEN FILE");
        return OPEN_FILE_ERROR;
    }

    /*

    ВОТ СЮДА ДОБАВИТЬ ЗАПИСЬ ХЭДЭРА-ПУСТЫШКИ

    */

    while (!feof(file))
    {
        sizeReadBlock = fread(block, sizeof(char), sizeBlock, file);

#ifdef DEBUG
        printf("\n--------------------------------\nCount of read bytes:\t%d\n", sizeReadBlock);
#endif
        coding(block, block, sizeReadBlock, block, &countCodingBits);

#ifdef DEBUG
        printf("Count of coding bytes:\t%d\n", countCodingBits / 8);
#endif

        for(i = 0; i < countCodingBits / 8; i++)
        {
            byteForWrite = partialByte;
            byteForWrite |= ((block[i] >> countUsedBits) & right1[8 - countUsedBits]);

            partialByte = ((block[i] << (8 - countUsedBits)) & left1[countUsedBits]);
#ifdef DEBUG
        printf("Byte for write:\t'%c'\n", byteForWrite);
#endif
            writeChar(archive, byteForWrite);
        }

        if (countCodingBits % 8)
        {
            if ((countUsedBits + countCodingBits % 8) >= 8)
            {
                byteForWrite = partialByte;
                byteForWrite |= ((block[countCodingBits / 8] >> countUsedBits) & right1[8 - countUsedBits]);

                partialByte = ((block[countCodingBits / 8] << (8 - countUsedBits)) & left1[countUsedBits]);

                writeChar(archive, byteForWrite);
            }
            else
            {
                partialByte |= ((block[countCodingBits / 8] >> countUsedBits) & right1[8 - countUsedBits] & left1[countUsedBits + countCodingBits % 8]);
                countUsedBits += countCodingBits % 8;
            }
        }



        strcpy(block, "");
    }

    if (countUsedBits)
    {
        writeChar(archive, partialByte);
    }
    archFileInfo.endUnusedBits = 8 - countUsedBits;

    /*

    ВОТ СЮДА ЗАПИСАТЬ УЖЕ ГОТОВЫЙ ХЭДЭР

    */


    dropWrBytes(archive);

    fclose(archive);
    fclose(file);

    return 0;
}


void coding(char* huffTree, char* bytesForCoding, int countBytesForCoding, char* codingBits, int* countCodingBits)
{
    memcpy(codingBits, bytesForCoding, countBytesForCoding);
    *countCodingBits = countBytesForCoding * 8;
}
