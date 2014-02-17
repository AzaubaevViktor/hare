#include <stdio.h>
#include <time.h>
#include <string.h>

#define GLOBAL_LOGGING \
  char __logging_file_name[80] = ""; \
  int __logging = 1;

#define INIT_LOGGING \
  time_t __logging_seconds = time(NULL); \
  struct tm *__logging_timeinfo = localtime(&__logging_seconds); \
  if (0 == strftime(__logging_file_name,80,"%d%m%Y_%H%M%S.log", __logging_timeinfo)) { \
    strcpy(__logging_file_name,"hare.log"); \
  } \
  FILE *__logging_file = fopen(__logging_file_name,"wt"); \
  if (NULL == __logging_file) { \
    fprintf(stderr, "ERROR! Logging file can't be open!"); \
    __logging = 0; \
  }

#define WRITE(str) \
  printf("`"__FILE__"`:%d: %s\n", __LINE__, str);

#define DEINIT_LOGGING \
  if (__logging) { \
    fclose(__logging_file); \
  }

GLOBAL_LOGGING

int test_func();

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

  test_func();

  DEINIT_LOGGING
  return 0;
}


int test_func() {
  WRITE("GET THE FUCKING FLOWERS!!!!!")
  return 0;
}
