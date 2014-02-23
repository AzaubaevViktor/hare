#include "logging.h"
#include "addfiles2arch.h"
#include <stdio.h>
#include <time.h>

GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
  INIT_LOGGING;
  FILE *f = fopen("test", "wb");
  int i = 0;
  writeBytes(f, "A я ебал гусяток\n", 0);
  writeBytes(f, "В рот\n", 0);
  writeBytes(f, "МНооооооооооооооооооооого разззззззззззззззззззз азазаз", 1);

  fclose(f);
  DEINIT_LOGGING;
  printf("Programm exit!\n");
  return 0;
}

