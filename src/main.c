#include "logging.h"
#include <stdio.h>
#include <time.h>
#include <string.h>


GLOBAL_LOGGING

#define TEST(str, ...) \
  printf(str, ##__VA_ARGS__);

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

  TEST("%s", "blah");

  printf("Op\n");
  return 0;
}


int test_func(const char * lol) {
  INFO("Opana %s", "__Ne ponyal na");
  MEMORY("Lol is: %s", lol);
  IO("IO-shechki bibib %d %d",12, 13);
  ERROR("A ya eby gusei");
  WARNING("op %s %s %s","musorok","!","Ne shei mne srok");

  return 0;
}
