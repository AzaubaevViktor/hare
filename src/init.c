/* by Alex Mukhin*/
#include "init.h"

int parseArgs(Context **context){

    char *str;
    char id;
    int i;
    FileNode *head = NULL;
    FileNode *iter;
    FILE *f = NULL;
    Context *cnt = *context;
    cnt->workFiles = (char **)malloc(sizeof(char *));
    if (cnt->argc < 3) {
        cnt->keys = 0x0;
        ERROR("Мало аргументов(1 этап)");
        printHelp();
        return -1;
    }
    str = cnt->argv[1];
    char *nameArch = cnt->argv[2];
    f = fopen(nameArch, "br");
    if (f != NULL) {
        cnt->archName = nameArch;
        fclose(f);
    }
    else {
        ERROR("Архив не удалось открыть");
        return -1;
    }
    if (*(str) == '-') {
      id = *(str+1);
      switch (id) {
          case 'a':
          if (cnt->argc < 4) {
              cnt->keys = 0x0;
              printHelp();
              ERROR("Мало аргументов(2 этап, case 'a'");
              break;
          }
              /*for (i=3;i<cnt->argc;i++){
                  f = fopen(cnt->argv[i], "r");
                  if (f != NULL){
                      if (head == NULL){
                          head       = (FileNode*)malloc(sizeof(FileNode));
                          iter       = head;
                      } else {
                          iter->next = (FileNode*)malloc(sizeof(FileNode));
                          iter       = iter->next;
                      }
                      *(cnt->workFiles + numFiles) = cnt->argv[i];
                      numFiles++;
                      cnt->workFiles = (char **)realloc(cnt->workFiles, (numFiles + 1)*sizeof(char *));
                      iter->file     = f;
                      iter->next     = NULL;
                      iter->info     = (FileInfo *)malloc(sizeof(FileInfo));
                  }
              }
              break;*/
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
              if ((f = fopen(cnt->argv[i], "r")) != NULL){
                  *(cnt->workFiles + i - 3) = cnt->argv[i];
                  iter->file     = f;
                  iter->next     = NULL;
                  iter->info     = (FileInfo *)malloc(sizeof(FileInfo));

              } else {
                  *(cnt->workFiles + i - 3) = NULL;
              }
              /*if (*(cnt->workFiles + i - 3) == NULL){
                  printf("NULL\n");
              } else {
                  printf("%s\n", *(cnt->workFiles + i - 3));
              }*/

          }
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

      }
    *context = cnt;
    return 0;
}
