#include "logging.h"
#include "hare.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

GLOBAL_LOGGING

#define TEST(str, ...) \
  printf(str, ##__VA_ARGS__);

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
  INFO("Call test func");
  test_func("Blah Blah Blah, Mr. Freeman");
  INFO("Test func stop");

  DEINIT_LOGGING;

  TEST("%s", "blah");

  printf("Op\n");
  return 0;
}
