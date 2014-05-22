#include  "addFile.h"

#define WRITE_HEADER
#define WRITE_CRC

#define PRINT_DATA_

#define DEBUG

static char* concatenateStrings(const char * str1, const char * str2)
{
    int lengthStr1 = strlen(str1);
    int i;
    char * result = (char*)calloc((strlen(str1) + strlen(str2) + 1), sizeof(char));

    if (NULL == result)
    {
        return NULL;
    }

    for (i = 0; i < lengthStr1; i++)
    {
        result[i] = str1[i];
    }
    for (i = 0; i < strlen(str2); i++)
    {
        result[i + lengthStr1] = str2[i];
    }

    return result;
}


void addFiles2Arch(Context context)
{
    int i;

#ifdef DEBUG
    printf("\n================ ADDING FILES TO ARCHIVE MOTHERFUCKER! =================\n\n\n");
#endif

    for (i = 0; i < context.argc - 3; i++)
    {
        struct stat fileInfo;
        stat(context.workFiles[i], &fileInfo);

        if (S_ISREG(fileInfo.st_mode))
        {
            ArchFileInfo archFileInfo;
            archFileInfo.fileInfo = (FileInfo*)malloc(sizeof(FileInfo));

            if (NULL == archFileInfo.fileInfo)
                continue;

            if (getFileInfo(pathToCanon(context.workFiles[i]), archFileInfo.fileInfo))
                continue;

            if (addFile2Arch(archFileInfo, context.archName))
                continue;

            free(archFileInfo.fileInfo);
        }
        else if (S_ISDIR(fileInfo.st_mode))
        {
#ifdef DEBUG
            printf("--------------------------------\n\n"
                   "INFO: Add folder '%s' to archive...\n", pathToCanon(concatenateStrings(context.workFiles[i], "/")));
#endif
            recurseAddFiles2Arch(context.workFiles[i], context);

#ifdef DEBUG
            printf("INFO: Folder '%s' was added!\n", pathToCanon(concatenateStrings(context.workFiles[i], "/")));
#endif
        }
    }

#ifdef DEBUG
    printf("\n\n================ ADDING FILES TO ARCHIVE FINISHED! =================\n\n\n");
#endif
}

int addFile2Arch(ArchFileInfo archFileInfo, const char* nameArchive)
{
    FILE* archive;
    FILE* file;

    struct Code codes[COUNT_SYMBOLS] = {0, 0};
    struct Node* headTree = NULL;

    const int sizeBlock = 30000;
    int sizeReadBlock;
    unsigned char block[sizeBlock];
    unsigned char codingBlock[sizeBlock];

    crc crcData = 0;
    int countCodingBits = 0;
    char byteForWrite;
    char countUsedBits = 0;
    char partialByte = 0;
    long positionHeaderInFile;


    char left1[9] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF};
    char right1[9] = {0, 1, 3, 7, 15, 31, 63, 127, 255};

    file     = fopen(archFileInfo.fileInfo->name,  "rb");
    if (!file)
        return OPEN_FILE_ERROR;


    archive  = fopen(nameArchive, "rb+");
    if (!archive)
    {
        archive  = fopen(nameArchive, "wb");
        if (!archive)
            return OPEN_ARCHIVE_ERROR;

        fclose(archive);
        archive  = fopen(nameArchive, "rb+");

    }

#ifdef DEBUG
            printf("--------------------------------\n\n"
                   "INFO: Add file '%s' to archive...\n", pathToCanon(archFileInfo.fileInfo->name));
            printFileInfo(*(archFileInfo.fileInfo));
#endif

    if (headTree = createTree(createList(createTableFrequencies(file))))
    {
        createCodes(codes, headTree, "");
        printCodes(codes);
    }


    fseek(file, 0L, SEEK_SET);
    fseek(archive, 0L, SEEK_END);
    positionHeaderInFile = ftell(archive);

    archFileInfo.haffTreeSize = 0;
    archFileInfo.haffTree = (char*)calloc(COUNT_SYMBOLS * 2 + 1, sizeof(char));
    writeHuffTreeInBuffer(headTree, archFileInfo.haffTree, &archFileInfo.haffTreeSize, &countUsedBits);
    archFileInfo.haffTreeSize = archFileInfo.haffTreeSize * 8 + countUsedBits;
    archFileInfo.dataSize = 0;
    archFileInfo.endUnusedBits = 0;

#ifdef WRITE_HEADER
    writeFileHeader(archive, &archFileInfo);
#endif

    countUsedBits = 0;
    memset(block, 0, sizeBlock);

    initWrCrc();

    while (!feof(file))
    {
        int i;

        sizeReadBlock = fread(block, sizeof(char), sizeBlock, file);

        coding(codes, block, sizeReadBlock, codingBlock, &countCodingBits);

#ifdef PRINT_DATA
        {
            int count = 0, j;
            for (i = 0; i < sizeReadBlock; i++)
            {
                for (j = 0; j < codes[block[i]].size; j++)
                {
                    printf("%c", codes[block[i]].code[j]);
                    count++;

                    if (!(count % 8))
                        printf(" ");

                    if (!(count % 32))
                        printf("\n");
                }
            }
        }
        printf("\n----------------------------------------\n");
#endif

        for(i = 0; i < countCodingBits / 8; i++)
        {
            byteForWrite = partialByte;
            byteForWrite |= ((codingBlock[i] >> countUsedBits) & right1[8 - countUsedBits]);
            partialByte = ((codingBlock[i] << (8 - countUsedBits)) & left1[countUsedBits]);

            writeChar(archive, byteForWrite);
            archFileInfo.dataSize++;
        }

        if (countCodingBits % 8)
        {
            if ((countUsedBits + countCodingBits % 8) >= 8)
            {
                byteForWrite = partialByte;
                byteForWrite |= ((codingBlock[countCodingBits / 8] >> countUsedBits) & right1[8 - countUsedBits]);
                partialByte = ((codingBlock[countCodingBits / 8] << (8 - countUsedBits)) & left1[countUsedBits]);

                writeChar(archive, byteForWrite);
                archFileInfo.dataSize++;
            }
            else
            {
                partialByte |= ((codingBlock[countCodingBits / 8] >> countUsedBits) & right1[8 - countUsedBits] & left1[countUsedBits + countCodingBits % 8]);
                countUsedBits += countCodingBits % 8;
            }
        }


        strcpy(block, "");
        memset(codingBlock, 0, sizeBlock);
    }

    if (countUsedBits)
    {
        writeChar(archive, partialByte);
        archFileInfo.dataSize++;
        archFileInfo.endUnusedBits = 8 - countUsedBits;
    }

#ifdef DEBUG
    printf("\nSize data: %d byte(s)\n", archFileInfo.dataSize);
#endif

#ifdef WRITE_CRC
    crcData = getWrCrc();
    writeCrc(archive, crcData);
#endif



#ifdef WRITE_HEADER
    fseek(archive, positionHeaderInFile, SEEK_SET);
    writeFileHeader(archive, &archFileInfo);
#endif

    free(archFileInfo.haffTree);

    fclose(archive);
    fclose(file);

#ifdef DEBUG
    printf("INFO: File '%s' was added!\n", pathToCanon(archFileInfo.fileInfo->name));
    getchar();
#endif

    return 0;
}




void recurseAddFiles2Arch(char * path, Context context)
{
    DIR * dir = NULL;
    struct dirent * dir_entry;
    ArchFileInfo archFileInfo;
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
        if (dir_entry->d_type == DT_REG)
        {
            // add file to arch
            archFileInfo.fileInfo = (FileInfo*)malloc(sizeof(FileInfo));

            if (NULL == archFileInfo.fileInfo)
                return;

            getFileInfo(concatenateStrings(concatenateStrings(buffer, "/"), dir_entry->d_name), archFileInfo.fileInfo);
            addFile2Arch(archFileInfo, context.archName);
        }
        else if (dir_entry->d_type == DT_DIR)
        {
            if (strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, ".."))
            {
                depth++;
                strcat(buffer, "/");
                strcat(buffer, dir_entry->d_name);
                recurseAddFiles2Arch(buffer, context);
            }
        }

    }

    if (depth > 0)
    {
        int i;
        buffer[strlen(buffer) - 1] = '\0';
        for (i = strlen(buffer) - 1; buffer[i] != '/'; i--)
            buffer[i] = '\0';
    }
    depth--;
}


