/* 2014 (c), Ktulhy-kun, велосипед олологирование
 * Как пользоваться:
 * Перед main вызываем GLOBAL_LOGGING
 * В main() нутри объявления переменных вызываем INIT_LOGGING
 * Для записи логов в файл использовать:
 * INFO/MEMORY/IO/ERROR/WARNING(format (не переменная, именно строка в кавычках), ...), при этом string -- уже готовая строка (форматирование как в printf нет)
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

#define __OUT(str, ...) \
  if (__logging) \
    fprintf(__logging_file, str, ##__VA_ARGS__);

#define WRITE(pre, str, ...) \
  if (__logging) { \
    __OUT(pre" `"__FILE__"`:%d: "str"\n", __LINE__, ##__VA_ARGS__); \
  }

#define INFO(str, ...) \
  WRITE(" [  INFO  ]", str, ##__VA_ARGS__); \

#define MEMORY(str, ...) \
  WRITE("[  MEMORY ]", str, ##__VA_ARGS__) \

#define IO(str, ...) \
  WRITE(" [   IO   ]", str, ##__VA_ARGS__)

#define ERROR(str, ...) \
  WRITE("[  ERROR  ]", str, ##__VA_ARGS__)

#define WARNING(str, ...) \
  WRITE("[ WARNING ]", str, ##__VA_ARGS__)


#define DEINIT_LOGGING \
  if (__logging) { \
    fclose(__logging_file); \
    __logging_file = NULL; \
  }

#endif // LOGGING_H
