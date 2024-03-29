/* by Alex Mukhin*/
#include "init.h"

int parseArgs(Context **context){

  LOGGING_FUNC_START;
  char *str;
  char id;
  int i;
  FileNode *head = NULL;
  FileNode *iter;
  Context *cnt = *context;
  cnt->workFiles = (char **)malloc(sizeof(char *));
  if (cnt->argc < 3) {
    cnt->keys = 0x0;
    ERROR(L"Мало аргументов(1 этап)");
    LOGGING_FUNC_STOP;
    return -1;
  }
  str = cnt->argv[1];
  cnt->archName = cnt->argv[2];

  if (*(str + 2) != '\0' || *(str) != '-'){
    cnt->keys = 0x0;
    ERROR(L"Плохой аргумент");
    LOGGING_FUNC_STOP;
    return -1;
  }

  id = *(str+1);
  switch (id) {
  case 'a':
    if (cnt->argc < 4) {
      cnt->keys = 0x0;
      ERROR(L"Мало аргументов(2 этап, case 'a'");
      break;
    }
    cnt->keys = 0x10;
    cnt->workFiles = (char **)calloc((cnt->argc - 2), sizeof(char*) );
    for (i = 3; i < cnt->argc; i++){
      if (head == NULL){
        head       = (FileNode*)calloc(1, sizeof(FileNode));
        iter       = head;
      } else {
        iter->next = (FileNode*)calloc(1, sizeof(FileNode));
        iter       = iter->next;
      }
      *(cnt->workFiles + i - 3) = cnt->argv[i];
      iter->next     = NULL;
      iter->fileInfo     = (FileInfo *)calloc(1, sizeof(FileInfo));
    }

    cnt->filesTree = head;
    break;
  case 'x':
    cnt->keys = 0x8;
    cnt->workFiles = (char **)calloc((cnt->argc - 2), sizeof(char*) );
    for (i = 3; i < cnt->argc; i++){
      *(cnt->workFiles + i - 3) = cnt->argv[i];
    }
    break;
  case 'd':
    cnt->keys = 0x4;
    cnt->workFiles = (char **)calloc((cnt->argc - 2), sizeof(char*) );
    for (i = 3; i < cnt->argc; i++){
      *(cnt->workFiles + i - 3) = cnt->argv[i];
    }
    break;
  case 'l':
    cnt->keys = 0x2;
    break;
  case 't':
    cnt->keys = 0x1;
    break;
  default:
    cnt->keys = 0x0;
    ERROR(L"Неверный параметр -%c", id);
    LOGGING_FUNC_STOP;
    return -1;
  }

  *context = cnt;
  LOGGING_FUNC_STOP;
  return 0;
}
