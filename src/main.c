#include "logging.h"
#include "addfiles2arch.h"
#include <stdio.h>
#include <time.h>

GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
  INIT_LOGGING;

  DEINIT_LOGGING;
  printf("Programm exit!\n");
  return 0;
}

