#include "logging.h"
#include "hare.h"
#include "init.h"
#include "path.h"
#include <stdio.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
  Context *cnt = (Context *)malloc(sizeof(Context));
  int _error = 0;
  INIT_LOGGING;
  LOGGING_FUNC_START;
  INFO(L"Programm started");
  INFO(L"Тест русского языка");
  printf("Hello World!\n");
  INFO(L"Programm ended");
  cnt->argv = argv;
  cnt->argc = argc;
  cnt->env  = env;
  _error = parseArgs(&cnt);

  printf("%s\n", pathToCanon("f"));
  printf("%s\n", pathToCanon("f/"));
  printf("%s\n", pathToCanon("/fold/"));
  printf("%s\n", pathToCanon("fold/fold"));
  printf("%s\n", pathToCanon("./fold/"));
  printf("%s\n", pathToCanon("/file"));
  printf("%s\n", pathToCanon("/"));

  printf("pathInDest\n");

  printf("%d\n", pathInDest("./file","./file"));
  printf("%d\n", pathInDest("./file","./file/."));
  printf("%d\n", pathInDest("./file/.","./file"));
  printf("%d\n", pathInDest("./file/.","./file/ololo"));
  printf("%d\n", pathInDest("/file/.","./file/ololo"));
  printf("%d\n", pathInDest("/file","./fileololo"));
  printf("%d\n", pathInDest("/file/aza","./file/azaololo"));

  printf("LEVELS\n");

  printf("%d\n", levels("./."));
  printf("%d\n", levels("./file"));
  printf("%d\n", levels("./fol/file"));
  printf("%d\n", levels("./fol/."));
  printf("%d\n", levels("./a/a/a/a/a/a/."));
  printf("%d\n", levels("./b/fi"));
  printf("%d\n", levels("/b/fi"));

  printf("getFileByPath\n");

  printf("%s\n", getFileByPath("./file", "./file"));
  printf("%s\n", getFileByPath("./file/.", "./file/."));
  printf("%s\n", getFileByPath("./fold/.", "./fold/file"));
  printf("%s\n", getFileByPath("./fold/.", "./fold/fold1/."));
  printf("%s\n", getFileByPath("./fold/.", "./fold/fold1/file"));
  printf("%s\n", getFileByPath("./fold", "./fold/file"));
  printf("%s\n", getFileByPath("./fold", "./fold/fold1/."));
  printf("%s\n", getFileByPath("./fold", "./fold/fold1/file"));
  printf("%s\n", getFileByPath("./fold/fold1", "./fold/fold1/."));
  printf("%s\n", getFileByPath("./fold/fold1", "./fold/fold1/file"));
  printf("%s\n", getFileByPath("./.", "./."));
  printf("%s\n", getFileByPath("./.", "./."));

    printf("Yiiiiiiii\n");

  if (0 != _error) {
    printHelp();
  } else {
    ;
  }
  char *str = pathToCanon("f/");
  printf("%s\n", str);
  DEINIT_LOGGING;
  printf("Programm exit!\n");
  printf("%s\n", *(cnt->workFiles));
  return 0;
}

