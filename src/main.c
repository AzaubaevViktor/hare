#include "logging.h"
#include "addfiles2arch.h"
#include <stdio.h>
#include <time.h>

GLOBAL_LOGGING;

int main(int argc, char *argv[], char *env[])
{
  INIT_LOGGING;
  FILE *f = fopen("test", "wb");
  unsigned char buf[BUF_LEN] = "Testii";
  writeBytes(f, buf,4);

  fclose(f);
  return 0;
  DEINIT_LOGGING;
}

