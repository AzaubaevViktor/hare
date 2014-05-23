#include <extract.h>

/* Декордирование
 * Принимает:
 *   bytes : Байтовую строку
 *   lenBit: Длинну в битах
 * Возвращает:
 *   returnBits : кол-во записанных бит
 */

#define min(a,b) (((a) < (b)) ? (a) : (b))


char *__forErrorFileName;

/* 0 -- continue
 * 1 -- stop */
int msg(int code) {
  switch (code) {
    case 0:
      return 0;
      break;

    case SIGNATURE_ERROR:
      printf("Signature error. Possibly you file damaged, check it with \n"
             "  hare -t filename\n"
             "Find next file... (NOT REALIZED)\n");
      WARNING(L"Signature error, find next (NOT REALIZED)");
      return 1; // POKA
      break;

    case IO_EOF:
      INFO("End of file\n");
      return 0;
      break;

     case MEMORY_ALLOCATE_ERROR:
      printf("Can not allocate memory. Close one or more programm to release memory.\n");
      MEMORY("Can'not allocate");
      return 1;
      break;

    case HASH_HEADER_CHECK_ERROR:
      //printf("Check summ file in archive does not match, search next file... (NOT REALIZED)\n");
      WARNING("Hash header check error");
      return 0; // POKA
      break;

    case MKDIR_ERROR:
      printf("Error when create folder `%s`\n", __forErrorFileName);
      __forErrorFileName = NULL;
      return 0;
      break;

    case FSEEK_ERROR:
      printf("Fseek error in archive `%s`\n", __forErrorFileName);
      ERROR("Fseek error");
      __forErrorFileName = NULL;
      return 1;
      break;

    case FILE_OPEN_ERROR:
      printf("Can not open file `%s` to write. Please, check file permission.\n", __forErrorFileName);
      __forErrorFileName = NULL;
      return 0;
      break;

    case ARCHIVE_ERROR:
      printf("Error while read archive\n");
      return 0;
      break;

    case IO_WRITE_ERROR:
      printf("Error while write data to `%s`\n", __forErrorFileName);
      __forErrorFileName = NULL;
      return 1;
      break;

    case OPEN_ARCHIVE_FILE_ERROR:
      printf("Error open archive file `%s`, please, check filename and permissions\n",__forErrorFileName);
      __forErrorFileName = NULL;
      return 1;
      break;

    case INCORRECT_FILE_NAME:
      printf("`%s` not found in archive\n", __forErrorFileName);
      __forErrorFileName = NULL;
      return 0;
      break;

    default:
      printf("Unknown error `%d`\n",code);
      return 1;
      break;
  }
}


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
    __forErrorFileName = fileName;
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

    _error = writeNBytes(fOut, returnBytes, buf2Write);

    if (_error) {
      IO("Write error to `%s`", fileName);
      __forErrorFileName = fileName;
      return _error;
    }
  }

  fclose(fOut);

  free(haffTree);
  LOGGING_FUNC_STOP;
  return 0;
}


#define ERROR_CHECK_NOT_LOOP\
  isEof += (IO_EOF == _error);\
  if (msg(_error)) {\
  LOGGING_FUNC_STOP;\
  return _error;\
  }

#define ERROR_CHECK\
  ERROR_CHECK_NOT_LOOP\
  if (isEof) break;

int extractFiles(FILE *f, Context *cnt) {
  char **files = cnt->workFiles;
  int64_t *count = NULL;
  char *res = NULL;
  char *currentFile = NULL;
  int64_t len = 0;
  int64_t i = 0;
  ArchFileInfo aFileInfo;
  FileInfo fInfo;
  int shifted = 0;
  int _error = 0;
  fpos_t archPos;
  bool isEof = 0;
  struct stat st = {0};
  LOGGING_FUNC_START;

  _error = f ? 0 : (__forErrorFileName = cnt->archName, OPEN_ARCHIVE_FILE_ERROR);
  ERROR_CHECK_NOT_LOOP;

  for (len=0; *(files+len); len++) {
    res = *(files + len);
    *(files + len) = pathToCanon(res);
  }

  count = calloc(len,sizeof(int64_t));

  INFO(L"NFiles:%d", len);
  aFileInfo.fileInfo = &fInfo;

  while (!isEof) {
    _error = readHeader(f, &aFileInfo);
    ERROR_CHECK;

    INFO(L"File in arch: %s", aFileInfo.fileInfo->name);

    shifted = 0;

    for (i=0; i<len; i++) {
      if ((*(files + i)) &&
          (pathInDest(*(files + i), aFileInfo.fileInfo->name))) {
        currentFile = getFileByPathWithFolder/*getFileByPath*/(*(files + i), aFileInfo.fileInfo->name);
        INFO(L"Current file: `%s`", currentFile);
        if (isFolder(currentFile)) {
          INFO(L"Folder");
          if (-1 == stat(currentFile, &st)) {
            currentFile[max((int64_t) strlen(currentFile)-2, 0)] = 0;
            INFO(L"Try to create folder `%s`", currentFile);
            _error = mkdir(currentFile, 0777);
            INFO(L"Folder create with error `%d`", _error);
            _error = _error ? (__forErrorFileName = currentFile, MKDIR_ERROR) : 0 ;
            ERROR_CHECK;
            _error = 0;
          }
          shifted = 1;
        } else {
          IO(L"Extract `%s` to `%s`", aFileInfo.fileInfo->name, currentFile);
          _error = extract(f, &aFileInfo, currentFile);
          INFO(L"Extract with error `%d`", _error);
          ERROR_CHECK;
          shifted = 1;
          count[i]++;
        }
        free(currentFile);
        currentFile = NULL;
      }
    }


    _error = fseek(f, (shifted ? 0 : (aFileInfo.dataSize))  + sizeof(crc), SEEK_CUR);
    INFO(L"fseek with error `%d`", _error);
    _error = _error ? (__forErrorFileName = cnt->archName ,FSEEK_ERROR) : 0;
    INFO("Position %d", (fgetpos(f, &archPos), archPos));
    INFO("==========================");
  }

  for (i=0; i<len; i++) {
    if (!count[i]) {
      __forErrorFileName = files[i];
      msg(INCORRECT_FILE_NAME);
    }
  }

  free(count);

  LOGGING_FUNC_STOP;
  return 0;
}
