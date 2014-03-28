#include "logging.h"
#include "archfiles.h"
#include "hare.h"
#include <stdio.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
  Context *cnt = (Context *)malloc(sizeof(Context));
  int test = 0;
  FILE *f = NULL;
  size_t read_bytes = 0;
  char str[100] = "";
  int _error = 0;


  INIT_LOGGING;
  LOGGING_FUNC_START;
  INFO(L"Programm started");
  INFO(L"Тест русского языка");
  printf("Hello World!\n");
  INFO(L"Programm ended");

  f = fopen("test_t","rt");
  do {
    _error = readNBytes(f, 10, str, &read_bytes);
    printf("%zd:`%s`\n", read_bytes, str);
    for (test=0; test<100; test++) {
      *(str+test) = 0;
    }
  } while (IO_EOF != _error);

  close(f);

  DEINIT_LOGGING;
  printf("Programm exit!\n");
  printf("%s\n", *(cnt->workFiles));
  return 0;
}

