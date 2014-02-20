#include <stdio.h>
#include <time.h>
#include <string.h>

#define GLOBAL_LOGGING \
  char __logging_file_name[80] = ""; \
  FILE *__logging_file = NULL;
  int __logging = 1; \
  char __logging_buf[1000] = "";

#define INIT_LOGGING \
  time_t __logging_seconds = time(NULL); \
  struct tm *__logging_timeinfo = localtime(&__logging_seconds); \
  if (0 == strftime(__logging_file_name,80,"%d%m%Y_%H%M%S.log", __logging_timeinfo)) { \
    strcpy(__logging_file_name,"hare.log"); \
  } \
  __logging_file = fopen(__logging_file_name,"wt"); \
  if (NULL == __logging_file) { \
    fprintf(stderr, "[  ERROR  ] Logging file can't be open!"); \
    __logging = 0; \
  }

#define __OUT(str) \
  if (__logging) \
    fprintf(__logging_file, str);

#define WRITE(pre, str) \
  if (__logging) { \
    sprintf(__logging_buf, "%s `"__FILE__"`:%d: %s\n", pre, __LINE__, str); \
    __OUT(__logging_buf); \
  }

#define INFO(str) \
  WRITE(" [  INFO  ]", str); \

#define MEMORY(str) \
  WRITE("[  MEMORY ]", str) \

#define IO(str) \
  WRITE(" [   IO   ]", str)

#define ERROR(str) \
  WRITE("[  ERROR  ]", str)

#define WARNING(str) \
  WRITE("[ WARNING ]", str)


#define DEINIT_LOGGING \
  if (__logging) { \
    fclose(__logging_file); \
  }

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

  DEINIT_LOGGING
  return 0;
}


int test_func(const char * lol) {
  char str[1000] = "";
  sprintf(str, "I fucked the bitch: '%s'",lol);
  INFO(str);
  MEMORY(str);
  IO(str);
  ERROR(str);
  WARNING(str);

  return 0;
}
