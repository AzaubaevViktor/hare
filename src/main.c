#include "logging.h"
#include "hare.h"
#include "init.h"

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

//  printList(createList(createTableFrequencies(fopen("test_file.txt", "rb"))));
//  printf("-----------------\n");
//  printTree(createTree(createList(createTableFrequencies(fopen("test_file.txt", "rb")))), 0);
//  printf("-----------------\n");
//    {
//        int lal;
//        char blockOrig[20] = "sasai", blockCod[20];
//        struct Code codes[COUNT_SYMBOLS] = {0, 0};
//        createCodes(codes, createTree(createList(createTableFrequencies(fopen("1.jpg", "rb")))), "");
//        printCodes(codes);

//        coding(codes, blockOrig, 5, blockCod, &lal);
//    }

    addFiles2Arch(*cnt);



//  {
//      int i;
//      int countBits = 0;
//      char buffer[30000] = {0};
//      int length = 0;
//      FILE * file = fopen("test_tree.txt", "wb");
//      struct Node* head = createTree(createList(createTableFrequencies(fopen("1.jpg", "rb"))));
//      printTree(head, 0);

//      writeHuffTreeInBuffer(head, buffer, &length, &countBits);

//      for (i = 0; i < length + (countBits ? 1 : 0); i++)
//        fprintf(file, "%c", buffer[i]);
//      fclose(file);
//  }
//    printList(createList(createTableFrequencies(fopen("1.jpg", "rb"))));
//  printTree(createTree(createList(createTableFrequencies(fopen("test_file.txt", "rb")))), 0);
  return 0;
}

