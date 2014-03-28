/* by Alex Mukhin*/
#include "init.h"

int parseArgs(Context **context){

    char *str;
    char id;
    int i;
    FileNode *head = NULL;
    FileNode *iter;
    Context *cnt = *context;
    cnt->workFiles = (char **)malloc(sizeof(char *));
    if (cnt->argc < 3) {
        cnt->keys = 0x0;
        ERROR("Мало аргументов(1 этап)");
        printHelp();
        return -1;
    }
    str = cnt->argv[1];
    cnt->archName = cnt->argv[2];

    if (*(str + 2) != '\0' || *(str) != '-'){
        cnt->keys = 0x0;
        ERROR("Плохой аргумент");
        printHelp();
        return -1;
    }

      id = *(str+1);
      switch (id) {
          case 'a':
          if (cnt->argc < 4) {
              cnt->keys = 0x0;
              printHelp();
              ERROR("Мало аргументов(2 этап, case 'a'");
              break;
          }
          cnt->keys = 0x10;
          cnt->workFiles = (char **)malloc((cnt->argc - 3)*sizeof(char*) );
          for (i = 3; i < cnt->argc; i++){
              if (head == NULL){
                  head       = (FileNode*)malloc(sizeof(FileNode));
                  iter       = head;
              } else {
                  iter->next = (FileNode*)malloc(sizeof(FileNode));
                  iter       = iter->next;
              }
              *(cnt->workFiles + i - 3) = cnt->argv[i];
              iter->next     = NULL;
              iter->fileInfo     = (FileInfo *)malloc(sizeof(FileInfo));
          }
          cnt->filesTree = head;
          break;
       case 'x':
          cnt->keys = 0x8;
          cnt->workFiles = (char **)malloc((cnt->argc - 3)*sizeof(char*) );
          for (i = 3; i < cnt->argc; i++){
              *(cnt->workFiles + i - 3) = cnt->argv[i];
          }
          break;
       case 'd':
          cnt->keys = 0x4;
          cnt->workFiles = (char **)malloc((cnt->argc - 3)*sizeof(char*) );
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
          ERROR("Неверный параметр -%c", id);
          break;
    }

    *context = cnt;
    return 0;
}
