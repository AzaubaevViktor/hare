#include "logging.h"
#include "hare.h"

#include "addFile.h"

#include <stdio.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
//  INIT_LOGGING;
//  LOGGING_FUNC_START;
//  INFO(L"Programm started");
//  INFO(L"Тест русского языка");
//  wprintf(L"Hello World!\n");
//  INFO(L"Programm ended");
//  LOGGING_FUNC_STOP;
//  DEINIT_LOGGING;
//  printf("Programm exit!\n");

    printf("%d\n", addFile2Arch());

  return 0;
}

