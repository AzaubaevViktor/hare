#include "deleteFileBlock.h"

void delBlock(const char *fileName, uint64_t offset, uint64_t origin)
{
    FILE *oldFile, *newFile;
    char tmpFileName[50];
    const uint64_t sizeBlock = 5;
    char block[sizeBlock];
    uint64_t pieceBlock = 0;
    uint64_t tmpSizeBlock = 0;


    tmpnam(tmpFileName);

    oldFile = fopen(fileName, "rb");
    newFile = fopen(tmpFileName + 5, "wb");

    if (NULL == newFile || NULL == oldFile)
    {
        /* ERROR! */
    }

    do {
        readNBytes(oldFile, sizeBlock, block, tmpSizeBlock);
        if ((ftell(oldFile) >= origin) && (ftell(oldFile) < origin + offset)) {
            pieceBlock = ftell(oldFile) - origin;
            fwrite(block, sizeof(char), sizeBlock - pieceBlock, newFile);
            fseek(oldFile, offset - (ftell(oldFile) - origin), SEEK_CUR);
        } else {
            fwrite(block, sizeof( char ), sizeBlock, newFile);
        }
    } while(tmpSizeBlock == sizeBlock);

    fclose(oldFile);
    fclose(newFile);

    remove(fileName);
    rename(tmpFileName + 5, fileName);
}
