#include "logging.h"
#include "archfiles.h"
#include "hare.h"
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
  int test = 0;
  FILE *f = NULL;
  size_t read_bytes = 0;
  char str[100] = "";
  int _error = 0;
  ArchFileInfo info;
  FileInfo fInfo;

  INIT_LOGGING;
  LOGGING_FUNC_START;
  INFO(L"Programm started");
  INFO(L"Тест русского языка");
  printf("Hello World!\n");
  INFO(L"Programm ended");

  f = fopen("test", "rb");

  info.fileInfo = &fInfo;
  readHeader(f, &info);

  extract(f, 72, &info, "ololol");

  fclose(f);

  LOGGING_FUNC_STOP;
  DEINIT_LOGGING;
  printf("Programm exit!\n");
  return 0;
}

