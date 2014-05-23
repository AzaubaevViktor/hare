#include  "addFile.h"

#define WRITE_HEADER
#define WRITE_CRC

#define PRINT_DATA

#define DEBUG


int writeFolderHeader(Context context, const char * folderName)
{
    FILE * archive;
    ArchFileInfo archFileInfo;
    crc crcFolder;

    archFileInfo.fileInfo = (FileInfo*) malloc(sizeof(FileInfo));

    if (NULL == archFileInfo.fileInfo)
        return ERROR_NOT_ALLOCATED_MEMORY;

    if (getFileInfo(pathToCanon(concatenateStrings(folderName, "/")), archFileInfo.fileInfo))
        return ERROR_GET_FILE_INFO;

    archFileInfo.fileInfo->size = 0;
    archFileInfo.dataSize = 0;
    archFileInfo.endUnusedBits = 0;
    archFileInfo.haffTreeSize = 0;

#ifdef DEBUG
    printf("--------------------------------\n\n"
           "INFO: Add folder '%s' to archive...\n", pathToCanon(concatenateStrings(folderName, "/")));

    printFileInfo(*(archFileInfo.fileInfo));
    getchar();
#endif

    if (NULL == (archive  = fopen(context.archName, "rb+")))
    {
        if (NULL == (archive  = fopen(context.archName, "wb")))
            return ERROR_OPEN_ARCHIVE;
        fclose(archive);

        if (NULL == (archive  = fopen(context.archName, "rb+")))
            return ERROR_OPEN_ARCHIVE;
    }

    fseek(archive, 0, SEEK_END);
    writeFileHeader(archive, &archFileInfo);

    initWrCrc();
    writeNBytes(archive, 0, NULL);
    crcFolder = getWrCrc();
    writeCrc(archive, crcFolder);

    fclose(archive);
    free(archFileInfo.fileInfo);

    return 0;
}


void addFiles2Arch(Context context)
{
    int i;
    int error = 0;
#ifdef DEBUG
    printf("\n================ ADDING FILES TO ARCHIVE MOTHERFUCKER! =================\n\n\n");
#endif

    for (i = 0; i < context.argc - 3; i++)
    {
        struct stat fileInfo;
        if (stat(context.workFiles[i], &fileInfo))
        {
            PRINT_ERROR(ERROR_GET_FILE_INFO, context.workFiles[i]);
            continue;
        }

        if (S_ISREG(fileInfo.st_mode))
        {
            ArchFileInfo archFileInfo;
            archFileInfo.fileInfo = (FileInfo*)malloc(sizeof(FileInfo));

            if (NULL == archFileInfo.fileInfo)
            {
                PRINT_ERROR(ERROR_NOT_ALLOCATED_MEMORY);
                continue;
            }

            if (error = getFileInfo(pathToCanon(context.workFiles[i]), archFileInfo.fileInfo))
            {
                PRINT_ERROR(error, context.workFiles[i]);
                continue;
            }

            if (addFile2Arch(archFileInfo, context.archName))
                continue;

            free(archFileInfo.fileInfo);
        }
        else if (S_ISDIR(fileInfo.st_mode))
        {
            if ('/' == context.workFiles[i][strlen(context.workFiles[i]) - 1])
                context.workFiles[i][strlen(context.workFiles[i]) - 1] = '\0';

            if (error = recurseAddFiles2Arch(context.workFiles[i], context))
                PRINT_ERROR(ERROR_NOT_ADD_FOLDER, context.workFiles[i]);
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

    int sizeReadBlock;
    unsigned char block[SIZE_BLOCK];
    unsigned char codingBlock[SIZE_BLOCK];

    crc crcData = 0;
    int countCodingBits = 0;
    unsigned char byteForWrite;
    char countUsedBits = 0;
    unsigned char partialByte = 0;
    long positionHeaderInFile;


    unsigned char left1[9] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF};
    unsigned char right1[9] = {0, 1, 3, 7, 15, 31, 63, 127, 255};

    file     = fopen(archFileInfo.fileInfo->name,  "rb");
    if (!file)
    {
        PRINT_ERROR(ERROR_OPEN_FILE, archFileInfo.fileInfo->name);
        return ERROR_OPEN_FILE;
    }

    archive  = fopen(nameArchive, "rb+");
    if (!archive)
    {
        archive  = fopen(nameArchive, "wb");
        if (!archive)
        {
            PRINT_ERROR(ERROR_OPEN_ARCHIVE, nameArchive);
            return ERROR_OPEN_ARCHIVE;
        }

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
#ifdef DEBUG
        printCodes(codes);
#endif
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
    memset(block, 0, SIZE_BLOCK);

    initWrCrc();

    while (!feof(file))
    {
        int64_t i;

        sizeReadBlock = fread(block, sizeof(char), SIZE_BLOCK, file);

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

        memset(codingBlock, 0, SIZE_BLOCK);
        memset(block, 0, SIZE_BLOCK);
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




int recurseAddFiles2Arch(char * path, Context context)
{
    int error = 0;

    DIR * dir = NULL;
    struct dirent * dir_entry;
    ArchFileInfo archFileInfo;
    static int depth = 0;
    static char buffer[1000000] = "";

    if (NULL == path)
        return ERROR_OPEN_FOLDER;

    dir = opendir(path);

    if (NULL == dir)
    {
        depth--;
        return ERROR_OPEN_FOLDER;
    }

    strcpy(buffer, path);

    if (error = writeFolderHeader(context, buffer))
        return error;

    while ((dir_entry = readdir(dir)) != NULL)
    {
        if (dir_entry->d_type == DT_REG)
        {
            // add file to arch
            archFileInfo.fileInfo = (FileInfo*)malloc(sizeof(FileInfo));

            if (NULL == archFileInfo.fileInfo)
                return ERROR_NOT_ALLOCATED_MEMORY;

            if (error = getFileInfo(pathToCanon(concatenateStrings(concatenateStrings(buffer, "/"), dir_entry->d_name)), archFileInfo.fileInfo))
                return error;
            if (error = addFile2Arch(archFileInfo, context.archName))
                return error;
        }
        else if (dir_entry->d_type == DT_DIR)
        {
            if (strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, ".."))
            {
                depth++;
                strcat(buffer, "/");
                strcat(buffer, dir_entry->d_name);
                if (error = recurseAddFiles2Arch(buffer, context))
                    return error;
            }
        }
    }

    if (depth > 0)
    {
        int64_t i;
        buffer[strlen(buffer) - 1] = '\0';
        for (i = strlen(buffer) - 1; buffer[i] != '/'; i--)
            buffer[i] = '\0';
    }
    depth--;
    return 0;
}


