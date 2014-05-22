#include <extract.h>

/* Декордирование
 * Принимает:
 *   bytes : Байтовую строку
 *   lenBit: Длинну в битах
 * Возвращает:
 *   returnBits : кол-во записанных бит
 */

#define min(a,b) (((a) < (b)) ? (a) : (b))


void _decoding(char *str, size_t lenBits, char *retStr, size_t *returnBytes, Tree *tree, int _drop) {
  static Tree *root = NULL;
  static Tree *twig = NULL;
  int64_t pos = 0;

  if (_drop) {
    twig = root = tree;
    return;
  }

  *returnBytes = 0;

  while (pos < lenBits) {
    while ((twig->isTwig) && (pos < lenBits)) {
      twig = _getbit(str, pos)
             ? twig->right
             : twig->left;
      pos++;
    }

    if (!(twig->isTwig)) {
      retStr[(*returnBytes)++] = twig->sym;
      twig = root;
    }
  }
}

void initDecoding(Tree *tree) {
  _decoding(NULL, 0, NULL, NULL, tree, 1);
}

void decode(char *str, size_t lenBits, char *retStr, size_t *returnBytes) {
  _decoding(str, lenBits, retStr, returnBytes, NULL, 0);
}


int extract(FILE *f, ArchFileInfo *info, char *fileName) {
  LOGGING_FUNC_START;
  int _error = 0;
  FILE *fOut = NULL;
  char *buf = malloc(BUF_SIZE*sizeof(char));
  char *buf2Write = malloc(BUF_SIZE*sizeof(char)*8);
  size_t lenBits = 0;
  size_t readBytes = 0;
  size_t returnBytes = 0;
  size_t readedBytes = 0;
  size_t howManyBytesRead = 0;
  Tree *haffTree = NULL;

  if (NULL == (fOut = fopen(fileName, "wb"))) {
    IO(L"Couldnt open file `%s`", fileName);
    LOGGING_FUNC_STOP;
    return FILE_OPEN_ERROR;
  }

  haffTree = decodeTree(info->haffTree,info->haffTreeSize);
  initDecoding(haffTree);

  for (readedBytes=0; readedBytes < info->dataSize;) {
    howManyBytesRead = min(BUF_SIZE, (info->dataSize - readedBytes));
    _error = readNBytes(f, howManyBytesRead, buf, &readBytes);

    readedBytes += readBytes;

    if (_error) {
      IO(L"Error reading archive file");
      LOGGING_FUNC_STOP;
      return ARCHIVE_ERROR;
    }

    lenBits = (howManyBytesRead < BUF_SIZE)
              ? readBytes*8 - info->endUnusedBits
              : readBytes*8;

    decode(buf,lenBits,buf2Write,&returnBytes);

    writeNBytes(fOut, returnBytes, buf2Write);

    if (_error) {
      WARNING(L"Read bytes error `%d`", _error);
      break;
    }
  }

  fclose(fOut);

  free(haffTree);

  LOGGING_FUNC_STOP;
  return _error;
}


#define ERROR_CHECK\
  switch (_error) {\
  case 0:\
  break;\
  case SIGNATURE_ERROR:\
  WARNING(L"Signature error, find next");\
  LOGGING_FUNC_STOP;\
  return _error;\
  break;\
  case IO_EOF:\
  INFO(L"End of file");\
  LOGGING_FUNC_STOP;\
  return 0;\
  default:\
  WARNING(L"readHeader return `%d` error", _error);\
  LOGGING_FUNC_STOP;\
  return _error;\
  break;\
  }


int extractFiles(FILE *f, Context *cnt) {
  char **files = cnt->workFiles;
  char *res = NULL;
  char *currentFile = NULL;
  int64_t len = 0;
  int64_t i = 0;
  ArchFileInfo aFileInfo;
  FileInfo fInfo;
  int shifted = 0;
  int _error = 0;
  fpos_t archPos;
  struct stat st = {0};
  LOGGING_FUNC_START;
  for (len=0; *(files+len); len++) {
    res = *(files + len);
    *(files + len) = pathToCanon(res);
  }

  INFO(L"Len:%d", len);
  aFileInfo.fileInfo = &fInfo;

  while (!feof(f)) {
    _error = readHeader(f, &aFileInfo);
    ERROR_CHECK;

    INFO(L"File in arch: %s", aFileInfo.fileInfo->name);

    shifted = 0;

    for (i=0; i<len; i++) {
      if ((*(files + i)) &&
          (pathInDest(*(files + i), aFileInfo.fileInfo->name))) {
        currentFile = /*getFileByPathWithFolder*/getFileByPath(*(files + i), aFileInfo.fileInfo->name);
        INFO(L"Current file: `%s`", currentFile);
        if (isFolder(currentFile)) {
          INFO(L"Folder");
          if (-1 == stat(currentFile, &st)) {
            currentFile[max((int64_t) strlen(currentFile)-2, 0)] = 0;
            INFO(L"Try to create folder `%s`", currentFile);
            _error = mkdir(currentFile, 0777);
            INFO(L"Folder create with error `%d`", _error);
            _error = 0;
          }
          shifted = 1;
        } else {
          IO(L"Extract `%s` to `%s`", aFileInfo.fileInfo->name, currentFile);
          _error = extract(f, &aFileInfo, currentFile);
          INFO(L"Extract with error `%d`", _error);
          ERROR_CHECK;
          shifted = 1;
          if (!isFolder(*(files + i))) {
            free(*(files+i));
            *(files + i) = NULL;
          }
        }
        free(currentFile);
        currentFile = NULL;
      }
    }


    fseek(f, (shifted ? 0 : (aFileInfo.dataSize))  + sizeof(crc), SEEK_CUR);
    INFO("Position %d", (fgetpos(f, &archPos), archPos));
    INFO("==========================");
  }

  LOGGING_FUNC_STOP;
  return 0;
}
