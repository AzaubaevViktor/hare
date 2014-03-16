#include "logging.h"
#include "hare.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
  FILE *f = NULL;
  char s[80] = "";
  time_t seconds = time(NULL);
  struct tm *timeinfo = localtime(&seconds);
  INIT_LOGGING;
  LOGGING_FUNC_START;

  printf("%d \n",timeinfo->tm_hour);
  strftime(s, 80, "%d%m%Y_%H%M%S", timeinfo);

  strcat(s,"_123");
  printf("%s \n", s);

  INFO("Call test func");
  test_func("Blah Blah Blah, Mr. Freeman");
  INFO("Test func stop");

  IO("LAL");
  MEMORY("LAL");

  INFO("LAlKa");
  test_func2();
  WARNING("WOW");

  INFO("Call test func second");
  test_func("Blah Blah Blah, Mr. Freeman");
  INFO("Test func stop!");

  LOGGING_FUNC_STOP;
  DEINIT_LOGGING;

  printf("Op\n");
  return 0;
}
