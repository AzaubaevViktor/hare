#include "logging.h"
#include "hare.h"
#include <stdio.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
  Context *cnt = (Context *)malloc(sizeof(Context));	  INIT_LOGGING;
  LOGGING_FUNC_START;
  INFO(L"Programm started");
  INFO(L"Тест русского языка");
  wprintf(L"Hello World!\n");
  INFO(L"Programm ended");
  cnt->argv = argv;
  cnt->argc = argc;
  cnt->env  = env;
  parseArgs(&cnt);
  DEINIT_LOGGING;
  wprintf(L"Programm exit!\n");
  wprintf(L"%s\n", *(cnt->workFiles));
  return 0;
}
