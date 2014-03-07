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
  size_t i = 0;

  while (!out) {
    out = readNBytes(f, 6, str, &read_b);
    printf("[(%d:%zd) ", out, read_b, str);
    for (i=0; i<read_b; i++) {
      printf("[%3d '%c']",str[i], str[i]);
    }
    printf("]\n");
  }
  fclose(f);
  DEINIT_LOGGING;
  printf("Programm exit!\x43\n");
  return 0;
}

