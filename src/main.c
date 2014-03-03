#include "logging.h"
#include "addfiles2arch.h"
#include <stdio.h>
#include <time.h>

GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
  INIT_LOGGING;
  FILE *f = fopen("test", "rb");
  size_t read_b = 0;
  char str[11] = "";
  int out = 0;
  while (!out) {
    out = readNBytes(f, 5, str, &read_b);
    printf("[(%d:%zd) '%s']", out, read_b, str);
  }
  fclose(f);
  DEINIT_LOGGING;
  printf("Programm exit!\x43\n");
  return 0;
}

