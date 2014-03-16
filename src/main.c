#include "logging.h"
#include "hare.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
  wchar_t s[80] = L"\0";
  time_t seconds = time(NULL);
  struct tm *timeinfo = localtime(&seconds);
  INIT_LOGGING;
  LOGGING_FUNC_START;

  wprintf(L"%lli \n",timeinfo->tm_hour);

  wcscat(s,L"s_cвв123ddвв\0");

//  s[wcslen(s)] = '\0';
  wprintf(L"%ls'\n", s);

  INFO(L"Зовём, зовём");
  test_func(L"Blah Blah Blah, Mr. Freeman");
  INFO(L"Test func stop");

  IO(L"LAL");
  MEMORY(L"LAL");

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
