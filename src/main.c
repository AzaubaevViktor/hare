#include "logging.h"
#include "addfiles2arch.h"
#include <stdio.h>
#include <time.h>

GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
  INIT_LOGGING;
  FILE *f = fopen("test", "wb");
  unsigned char buf[BUF_LEN+1] = "12345678";
  writeBytes(f, buf, BUF_LEN);
  writeBytes(f, buf, 4);

  fclose(f);
  DEINIT_LOGGING;
  printf("Programm exit!\n");
  return 0;
}

