/* 2014 (c), Ktulhy-kun, велосипед олологирование
 *
 * КАК ПОЛЬЗОВАТЬСЯ:
 * Перед main вызываем GLOBAL_LOGGING
 * В main() нутри объявления переменных вызываем INIT_LOGGING
 * В начале функции после объявления переменных написать LOGGING_FUNC_START
 * Перед КАЖДЫМ return писать LOGGING_FUNC_STOP
 *
 * ДЛЯ ЗАПИСИ ЛОГОВ В ФАЙЛ ИСПОЛЬЗОВАТЬ:
 * INFO/MEMORY/IO/ERROR/WARNING(format (не переменная, именно строка в кавычках), ...), при этом string -- уже готовая строка (форматирование как в printf нет)
 * В конце программы перед самым return писать DEINIT_LOGGING
 * Файл будет называться [дата слитно]_[время слитно].log
 *
 * МОЖНО ОПРЕДЕЛЯТЬ УРОВЕНЬ ЛОГИРОВАНИЯ НА ЭТАПЕ КОМПИЛЯЦИЯ:
 * DEBUG включает все режимы логирования
 * DEBUG_[MODE] определяет, будет ли поступать в лог файл та или иная отладочная информация
 * Для undefine достаточно добавить после режима "_" без кавычек
 */


#ifndef LOGGING_H
#define LOGGING_H

#include <string.h>
#include <stdio.h> //Стандартный однобайтовый ввод-вывод
#include <wchar.h> //"Широкие" многобайтовые символы и их ввод-вывод
#include <wctype.h> //"Классификация" широких символов
#include <locale.h> //Во избежание "крокозябр" на выводе

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
extern int __logging_level;
extern wchar_t __logging_pre_tree[1000];
#endif


#ifdef DEBUG
#define GLOBAL_LOGGING \
  char __logging_file_name[80] = ""; \
  FILE *__logging_file = NULL; \
  int __logging = 1; \
  int __logging_level = 0; \
  wchar_t __logging_pre_tree[1000] = L"";
#else
#define GLOBAL_LOGGING
#endif


#ifdef DEBUG
#define INIT_LOGGING \
  time_t __logging_seconds = time(NULL); \
  struct tm *__logging_timeinfo = localtime(&__logging_seconds); \
  __logging_level = 0; \
  int __logging_count = 0; \
  \
  if (! setlocale(LC_ALL, "ru_RU.utf8")) {\
    printf("ERROR: console can't UTF8, lol"); \
    return 1; \
  }\
  for (__logging_count=0; __logging_count<1000; __logging_count++) { \
    __logging_pre_tree[__logging_count] = 0; \
  } \
  if (0 == strftime(__logging_file_name,80,"%Y%m%d_%H%M%S.log", __logging_timeinfo)) { \
    strcpy(__logging_file_name,"hare.log"); \
  } \
  __logging_file = fopen(__logging_file_name,"wt"); \
  if (NULL == __logging_file) { \
    fwprintf(stderr, L"[  ERROR  ] Logging file can't be open!"); \
    __logging = 0; \
  }\
  __OUT(L"PROGRAMM\n");

#else
#define INIT_LOGGING
#endif


#ifdef DEBUG
#define __OUT(str, ...) \
  if (__logging) \
    fwprintf(__logging_file, str, ##__VA_ARGS__);
#else
#define __OUT(...)
#endif


#ifdef DEBUG
#define LOGGING_FUNC_START \
  __logging_level += 1; \
  if (__logging_level) {\
    __logging_pre_tree[wcslen(__logging_pre_tree) - 1] = 0; \
  } \
  __OUT(L"%ls│\n%ls├─`"__FILE__ L"`:%d:\n", __logging_pre_tree, __logging_pre_tree, __LINE__); \
  if (__logging_level) { \
    wcscat(__logging_pre_tree, L"│  ├"); \
  }
#else
#define LOGGING_FUNC_START
#endif


#ifdef DEBUG
#define LOGGING_FUNC_STOP \
  __logging_pre_tree[wcslen(__logging_pre_tree) - 1] = 0; \
  __OUT(L"%ls└────────\n%ls\n", __logging_pre_tree, __logging_pre_tree); \
  if (__logging_level) { \
    __logging_pre_tree[wcslen(__logging_pre_tree) - 3] = 0; \
    __logging_level -= 1; \
  } \
  wcscat(__logging_pre_tree,L"├");
#else
#define LOGGING_FUNC_STOP
#endif


#ifdef DEBUG
#define WRITE(pre, str, ...) \
    __OUT(L"%ls "pre L""str L"\n", __logging_pre_tree, ##__VA_ARGS__);
#else
#define WRITE(...)
#endif


#ifdef DEBUG_INFO
#define INFO(str, ...) \
  WRITE(L"INFO   : ", str, ##__VA_ARGS__);
#else
#define INFO(...)
#endif


#ifdef DEBUG_MEMORY
#define MEMORY(str, ...) \
  WRITE(L"MEMORY : ", str, ##__VA_ARGS__);
#else
#define MEMORY(...)
#endif


#ifdef DEBUG_IO
#define IO(str, ...) \
  WRITE("IO     : ", str, ##__VA_ARGS__);
#else
#define IO(...)
#endif


#ifdef DEBUG_ERROR
#define ERROR(str, ...) \
  WRITE(L"ERROR  : ", str, ##__VA_ARGS__);
#else
#define ERROR(...)
#endif


#ifdef DEBUG_WARNING
#define WARNING(str, ...) \
  WRITE(L"WARNING: ", str, ##__VA_ARGS__);
#else
#define WARNING(...)
#endif

#ifdef DEBUG
#define EMPTY \
  WRITE(L"",L"",L"");
#else
#define EMPTY
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
