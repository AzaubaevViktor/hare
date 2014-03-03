#include "init.h"

int parseArgs(Context **context){

    char *str;
    char id;
    int i; int numFiles=0;
    FileNode *head = NULL;
    FileNode *iter;
    FILE *f = NULL;
    Context *cnt = *context;
    cnt->workFiles = (char **)malloc(sizeof(char *));
    if (cnt->argc == 1) {
        ERROR("Мало аргументов(1 этап)");
        printHelp();
        return;
    }
    str = cnt->argv[1];
    if (*(str) == '-') {
      id = *(str+1);
      switch (id) {
          case 'a':
          if (cnt->argc < 4) {
              printHelp();
              ERROR("Мало аргументов(2 этап, case 'a'");
              break;
          }
              for (i=3;i<cnt->argc;i++){
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
              break;
        case 'x'://Извлечение файлов
            break;
        case 'd'://Удаление файлов из архива
            break;
        case 'l'://Вывести всю поднаготную о файлах
            break;
        case 't'://Целостность архива
            break;
        default:
            break;
          }
      *(cnt->workFiles + numFiles) = (char *)calloc(1, sizeof(char));//Как бы нулевой символ
      }
    *context = cnt;

}
