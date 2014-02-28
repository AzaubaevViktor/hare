/* 2014 (c), Ktulhy-kun, велосипед олологирование
 * Как пользоваться:
 * Перед main вызываем GLOBAL_LOGGING
 * В main() нутри объявления переменных вызываем INIT_LOGGING
 * Для записи логов в файл использовать:
 * INFO/MEMORY/IO/ERROR/WARNING(format (не переменная, именно строка в кавычках), ...), при этом string -- уже готовая строка (форматирование как в printf нет)
 * В конце программы перед самым return писать DEINIT_LOGGING
 * Файл будет называться [дата слитно]_[время слитно].log
 * Можно определять уровень олологирования на уровне компиляции:
 * DEBUG включает все режимы логирования
 * DEBUG_[MODE] определяет, будет ли поступать в лог файл та или иная отладочная информация
 * Для undefine достаточно добавить после режима "_" без кавычек
 */
#ifndef LOGGING_H
#define LOGGING_H

#include <string.h>
#include <stdio.h>
#include <time.h>
#define DEBUG

#ifdef DEBUG
#define DEBUG_INFO
#define DEBUG_MEMORY
#define DEBUG_IO
#define DEBUG_ERROR
#define DEBUG_WARNING
#endif

#ifdef DEBUG
extern char __logging_file_name[80];
extern FILE *__logging_file;
extern int __logging;
#endif

#ifdef DEBUG
#define GLOBAL_LOGGING \
  char __logging_file_name[80] = ""; \
  FILE *__logging_file = NULL; \
  int __logging = 1;
#else
#define GLOBAL_LOGGING
#endif

#ifdef DEBUG
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
#else
#define INIT_LOGGING
#endif

#ifdef DEBUG
#define __OUT(str, ...) \
  if (__logging) \
    fprintf(__logging_file, str, ##__VA_ARGS__);
#else
#define __OUT(...)
#endif

#ifdef DEBUG
#define WRITE(pre, str, ...) \
    __OUT(pre" `"__FILE__"`:%d: "str"\n", __LINE__, ##__VA_ARGS__);
#else
#define WRITE(...)
#endif

#ifdef DEBUG_INFO
#define INFO(str, ...) \
  WRITE(" [  INFO  ]", str, ##__VA_ARGS__);
#else
#define INFO(...)
#endif

#ifdef DEBUG_MEMORY
#define MEMORY(str, ...) \
  WRITE("[  MEMORY ]", str, ##__VA_ARGS__);
#else
#define MEMORY(...)
#endif

#ifdef DEBUG_IO
#define IO(str, ...) \
  WRITE(" [   IO   ]", str, ##__VA_ARGS__);
#else
#define IO(...)
#endif

#ifdef DEBUG_ERROR
#define ERROR(str, ...) \
  WRITE("[  ERROR  ]", str, ##__VA_ARGS__);
#else
#define ERROR(...)
#endif

#ifdef DEBUG_WARNING
#define WARNING(str, ...) \
  WRITE("[ WARNING ]", str, ##__VA_ARGS__);
#else
#define WARNING(...)
#endif

#ifdef DEBUG
#define DEINIT_LOGGING \
  if (__logging) { \
    fclose(__logging_file); \
    __logging_file = NULL; \
  }
#else
#define DEINIT_LOGGING
#endif

#endif // LOGGING_H
