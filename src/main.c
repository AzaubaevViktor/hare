#include "logging.h"
#include <stdio.h>
#include <time.h>
#include <string.h>


GLOBAL_LOGGING

int test_func(const char *lol);

int main(int argc, char *argv[], char *env[])
{
  FILE *f = NULL;
  char s[80] = "";
  time_t seconds = time(NULL);
  struct tm *timeinfo = localtime(&seconds);
  INIT_LOGGING

  printf("%d \n",timeinfo->tm_hour);
  strftime(s, 80, "%d%m%Y_%H%M%S", timeinfo);

  printf("%s \n", s);

  test_func("Blah Blah Blah, Mr. Freeman");

  DEINIT_LOGGING;
  printf("Op\n");
  return 0;
}


int test_func(const char * lol) {
  char str[1000] = "";
  sprintf(str, "I fucked the bitch: '%s'",lol);
  INFO(str);
  MEMORY(str);
  IO("IO-shechki bibib");
  ERROR(str);
  WARNING(str);

  return 0;
}
