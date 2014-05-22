#include "logging.h"
#include "hare.h"
#include "init.h"
#include "path.h"
#include "extract.h"
#include <stdio.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
  Context *cnt = (Context *)malloc(sizeof(Context));
  FILE *f = NULL;
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

  printf("%d\n",_getchar("123",8));
  printf("%d\n",_getchar("123",9));
  printf("%d\n",_getchar("123",10));
  printf("%d\n",_getchar("123",11));
  printf("%d\n",_getchar("123",12));

  if (0 != _error) {
    printHelp();
  } else {
    switch(cnt->keys) {
    case 0x0:
      ;
      break;
    case 0x1:
      ;
      break;
    case 0x2:
      ;
      break;
    case 0x4:
      ;
      break;
    case 0x8:
      f = fopen(argv[2], "rb");
      extractFiles(f, cnt);
      fclose(f);
      break;
    case 0x10:
      ;
      break;
    default:
      printHelp();
      break;
    }
  }

  LOGGING_FUNC_STOP;
  DEINIT_LOGGING;
  printf("Programm exit!\n");
  return 0;
}

