#include  "addFile.h"

#define DEBUG_

int addFiles2Arch(Context context, int recurse)
{
    int i;
    struct stat fileInfo;
    struct ArchFileInfo archFileInfo;

    for (i = 0; i < countWorkFiles/* HUI ZNAET GDE LAL */; i++)
    {
        stat(context.workFiles[i], &fileInfo);

        if (S_ISREG(fileInfo.st_mode))
        {
            archFileInfo.fileInfo = (FileInfo*)malloc(sizeof(FileInfo));

            if (NULL == archFileInfo.fileInfo)
            {
                return -1;
            }

            getFileInfo(context.workFiles[i], archFileInfo.fileInfo);

            addFile2Arch(archFileInfo, context.archName);
        }
        else if (recurse && S_ISDIR(fileInfo.st_mode))
        {
            recurseAddFiles2Arch(context.workFiles[i]);
        }
    }

    return 0;
}

int addFile2Arch(ArchFileInfo archFileInfo, const char* nameArchive)
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

    long positionHeaderInFile;

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

    positionHeaderInFile = ftell(archive);

    archFileInfo.dataSize = 0;
    archFileInfo.endUnusedBits = 0;
    archFileInfo.haffTreeSize = 0;
    strcpy(archFileInfo.haffTree, "");
    archFileInfo.HeaderCheckSum = 0;

    writeFileHeader(archive, &archFileInfo);

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

    dropWrBytes(archive);


    fseek(archive, positionHeaderInFile, SEEK_SET);

    writeFileHeader(archive, &archFileInfo);


    fclose(archive);
    fclose(file);

    return 0;
}


void coding(char* huffTree, char* bytesForCoding, int countBytesForCoding, char* codingBits, int* countCodingBits)
{
    memcpy(codingBits, bytesForCoding, countBytesForCoding);
    *countCodingBits = countBytesForCoding * 8;
}

void recurseAddFiles2Arch(char * path)
{
    DIR * dir = NULL;
    struct dirent * dir_entry;
    struct stat file_info;

    struct FileInfo fileInfo2Arch;
    struct ArchFileInfo archFileInfo;

    int i;

    static int depth = 0;
    static char buffer[1000000] = "";

    if (NULL == path)
        return;

    dir = opendir(path);

    if (NULL == dir)
    {
        depth--;
        return;
    }

    strcpy(buffer, path);

    while ((dir_entry = readdir(dir)) != NULL)
    {
        stat(dir_entry->d_name, &file_info);

        if (S_ISDIR(file_info.st_mode))
        {
            if (strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, ".."))
            {
                depth++;
                strcat(buffer, dir_entry->d_name);
                strcat(buffer, "/");
                sasai(buffer);
            }
        }
        else if (S_ISREG(file_info.st_mode))
        {
            // add file to arch
            archFileInfo.fileInfo = (FileInfo*)malloc(sizeof(FileInfo));

            if (NULL == archFileInfo.fileInfo)
            {
                return -1;
            }

            getFileInfo(concatenateStrings(buffer, concatenateStrings("/", dir_entry->d_name)), archFileInfo.fileInfo);

            addFile2Arch(archFileInfo, context.archName);
        }
    }

    if (depth > 0)
    {
        buffer[strlen(buffer) - 1] = '\0';
        for (i = strlen(buffer) - 1; buffer[i] != '/'; i--)
            buffer[i] = '\0';
    }
    depth--;
}

static char* concatenateStrings(const char * str1, const char * str2)
{
    int lengthStr1 = strlen(str1);
    int i;
    char * result = (char*)calloc((strlen(str1) + strlen(str2) + 1) * sizeof(char));

    if (NULL == result)
    {
        return NULL;
    }

    for (i = 0; i < lengthStr1; i++)
    {
        result[i] = str1[i];
    }
    for (int i = 0; i < strlen(str2); i++)
    {
        result[i + lengthStr1] = str2[i];
    }

    return result;
}
