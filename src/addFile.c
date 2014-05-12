#include  "addFile.h"

#define DEBUG_


int addFiles2Arch(Context context, int recurse)
{
    int i;

    for (i = 0; i < context.argc - 3; i++)
    {
        struct stat fileInfo;
        stat(context.workFiles[i], &fileInfo);

        if (S_ISREG(fileInfo.st_mode))
        {
            ArchFileInfo archFileInfo;
            archFileInfo.fileInfo = (FileInfo*)malloc(sizeof(FileInfo));

            if (NULL == archFileInfo.fileInfo)
                return -1;

            getFileInfo(context.workFiles[i], archFileInfo.fileInfo);

            addFile2Arch(archFileInfo, context.archName);
        }
        else if (recurse && S_ISDIR(fileInfo.st_mode))
        {
            recurseAddFiles2Arch(context.workFiles[i], context);
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


    char left1[9] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF};
    char right1[9] = {0, 1, 3, 7, 15, 31, 63, 127, 255};


    strcpy(block, "");


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

    fseek(archive, 0L, SEEK_END);
    positionHeaderInFile = ftell(archive);


    archFileInfo.dataSize = 0;
    archFileInfo.endUnusedBits = 0;
    archFileInfo.haffTreeSize = 0;
    archFileInfo.haffTree = (char*)calloc(archFileInfo.haffTreeSize + 1, sizeof(char));
    strcpy(archFileInfo.haffTree, "");
    archFileInfo.HeaderCheckSum = 0;


    writeFileHeader(archive, &archFileInfo);
    dropWrBytes(archive);


    while (!feof(file))
    {
        int i;

        sizeReadBlock = fread(block, sizeof(char), sizeBlock, file);
        coding(block, block, sizeReadBlock, block, &countCodingBits);

        for(i = 0; i < countCodingBits / 8; i++)
        {
            byteForWrite = partialByte;
            byteForWrite |= ((block[i] >> countUsedBits) & right1[8 - countUsedBits]);

            partialByte = ((block[i] << (8 - countUsedBits)) & left1[countUsedBits]);
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
        writeChar(archive, partialByte);
    dropWrBytes(archive);


    archFileInfo.endUnusedBits = 8 - countUsedBits;


    fseek(archive, positionHeaderInFile, SEEK_SET);
    writeFileHeader(archive, &archFileInfo);
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

            getFileInfo(strcat(buffer, dir_entry->d_name), archFileInfo.fileInfo);
            addFile2Arch(archFileInfo, context.archName);
        }
        else if (dir_entry->d_type == DT_DIR)
        {
            if (strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, ".."))
            {
                depth++;
                strcat(buffer, dir_entry->d_name);
                strcat(buffer, "/");
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


