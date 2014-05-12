#include "logging.h"
#include "hare.h"
#include "init.h"

#include "huff.h"

#include "addFile.h"

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

  if (0 != _error) {
    printHelp();
  } else {
    ;
  }

  DEINIT_LOGGING;
  printf("Programm exit!\n");
  //printf("%s\n", *(cnt->workFiles));

//  printList(createList(createTableFrequencies(fopen("file.txt", "rb"))));
//  printf("-----------------\n");
//  printTree(createTree(createList(createTableFrequencies(fopen("file.txt", "rb")))), 0);


  addFiles2Arch(*cnt, 0);



  return 0;
}

