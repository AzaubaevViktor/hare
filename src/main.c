#include "logging.h"
#include "hare.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdio.h> //Стандартный однобайтовый ввод-вывод
#include <wchar.h> //"Широкие" многобайтовые символы и их ввод-вывод
#include <wctype.h> //"Классификация" широких символов
#include <locale.h> //Во избежание "крокозябр" на выводе

GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
  char s[80] = "\0";
  wchar_t ls[80] = L"\0";
  time_t seconds = time(NULL);
  struct tm *timeinfo = localtime(&seconds);
  INIT_LOGGING;
  LOGGING_FUNC_START;

  wprintf(L"%lli \n",timeinfo->tm_hour);

//  wcscat(s,L"s_cвв123ddвв\0");

//  s[wcslen(s)] = '\0';
//  wprintf(L"%ls'\n", s);

  strftime(s, 80, "%Y%m%d_%H%M%S.log", timeinfo);


  INFO(L"Текущие дата и время: %s", s);
  swprintf(ls, sizeof(ls) / sizeof(*ls), L"%s", s);

  INFO(L"После преобразования %ls", ls);

  INFO(L"Зовём, зовём");
  test_func(L"Blah Blah Blah, Mr. Freeman");
  INFO(L"Test func stop");

  IO(L"LAL");
  MEMORY(L"LAL");

  EMPTY;

  INFO(L"LAlKa");
  test_func2();
  WARNING(L"WOW");

  INFO(L"Call test func second");
  test_func(L"Blah Blah Blah, Mr. Freeman");
  INFO(L"Test func stop!");

  LOGGING_FUNC_STOP;
  DEINIT_LOGGING;

  wprintf(L"Op\n");
  return 0;
}
