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
  static int64_t pos = 0;
  int codeLen = 0;

  if (_drop) {
    twig = root = tree;
    pos = 0;
    return;
  }

  *returnBytes = 0;

  while (lenBits<=pos) {
    codeLen = 0;
    while ((twig->type) && (lenBits<=pos)) {
      twig = _getbit(str, pos)
             ? twig->right
             : twig->left;
      codeLen++;
      pos++;
    }

    if (!(twig->type)) {
      retStr[(*returnBytes)++] = twig->sym;
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
  int dropBuf = 0;
  size_t lenBits = 0;
  size_t readBytes = 0;
  size_t returnBytes = 0;
  size_t readedBytes = 0;
  size_t howManyBytesRead = 0;
  struct utimbuf times;

  if (NULL == (fOut = fopen(fileName, "wb"))) {
    IO(L"Couldnt open file `%s`", fileName);
    LOGGING_FUNC_STOP;
    return FILE_OPEN_ERROR;
  }

  dropWrBytes(fOut);

  for (readedBytes=0; readedBytes < info->dataSize;) {
    howManyBytesRead = min(BUF_SIZE, (info->dataSize - readedBytes));
    _error = readNBytes(f, howManyBytesRead, buf, &readBytes);

    readedBytes += readBytes;

    if ((howManyBytesRead == BUF_SIZE) & (_error)) {
      IO(L"Error reading archive file");
      LOGGING_FUNC_STOP;
      return ARCHIVE_ERROR;
    }

    lenBits = (howManyBytesRead < BUF_SIZE)
              ? ((dropBuf = 1), readBytes*8 - info->endUnusedBits)
              : ((dropBuf = 0), readBytes*8);

    buf2Write = decoding(buf, lenBits, &returnBytes, dropBuf);

    writeNBytes(fOut, returnBytes, buf2Write);

    if (_error) {
      WARNING(L"Read bytes error `%d`", _error);
      break;
    }
  }

  dropWrBytes(fOut);

  fclose(fOut);

  times.actime = info->fileInfo->timeLastAccess;
  times.modtime = info->fileInfo->timeLastModification;
  printf("%d", utime(fileName, &times));

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

  while (IO_EOF != _error) {
    _error = readHeader(f, &aFileInfo);
    ERROR_CHECK;

    INFO(L"File in arch: %s", aFileInfo.fileInfo->name);

    shifted = 0;

    for (i=0; i<len; i++) {
      if ((*(files + i)) &&
          (pathInDest(*(files + i), aFileInfo.fileInfo->name))) {
        currentFile = getFileByPathWithFolder(*(files + i), aFileInfo.fileInfo->name);
        INFO(L"Current file: `%s`", currentFile);
        if (isFolder(currentFile)) {
          INFO(L"Folder");
          if (-1 == stat(currentFile, &st)) {
            currentFile[max(strlen(currentFile)-2, 0)] = 0;
            _error = mkdir(currentFile, 0777);
            INFO(L"Folder create with error `%d`", _error);
            _error = 0;
          }
          shifted = 1;
        } else {
          IO(L"Extract `%s` to `%s`", aFileInfo.fileInfo->name, currentFile);
          _error = extract(f, &aFileInfo, currentFile);
          ERROR_CHECK;
          INFO(L"Extract with error `%d`", _error)
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

    if (!shifted) {
      fgetpos(f, &archPos);
      fseek(f, archPos.__pos - (BUF_LEN - getRdPos(f)) + aFileInfo.dataSize, SEEK_SET);
      dropRdBytes(f);
      INFO(L"Position: `%d` + `%d` ", (archPos.__pos - (BUF_LEN - getRdPos(f))), aFileInfo.dataSize);
    }
  }

  LOGGING_FUNC_STOP;
  return 0;
}
