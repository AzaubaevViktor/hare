#include "logging.h"
#include "addfiles2arch.h"
#include <stdio.h>
#include <time.h>

GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
  INIT_LOGGING;
  FILE *f = fopen("test", "rb");
  char str[5] = "";
  int out = 0;
  while (!out) {
    out = readNBytes(f, 4, str);
    printf("(%d) '%s'\n", out, str);
  }
  fclose(f);
  DEINIT_LOGGING;
  printf("Programm exit!\x43\n");
  return 0;
}

