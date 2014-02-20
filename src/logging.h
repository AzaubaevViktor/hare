/* 2014 (c), Ktulhy-kun, велосипед олологирование
 * Как пользоваться:
 * Перед main вызываем GLOBAL_LOGGING
 * В main() нутри объявления переменных вызываем INIT_LOGGING
 * Для записи логов в файл использовать:
 * INFO/MEMORY/IO/ERROR/WARNING(string), при этом string -- уже готовая строка (форматирование как в printf нет)
 * В конце программы перед самым return писать DEINIT_LOGGING
 * Файл будет называться [дата слитно]_[время слитно].log
 */
#ifndef LOGGING_H
#define LOGGING_H

#include <string.h>
#include <stdlib.h>

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
  fprintf(__logging_file, "%s", str);

#define WRITE(pre, str) \
  if (__logging) { \
    sprintf(__logging_buf, pre" `"__FILE__"`:%d: %s\n", __LINE__, str); \
    __OUT(__logging_buf); \
    strcpy(__logging_buf, ""); \
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
    __logging_file = NULL; \
  }

#endif // LOGGING_H
