#include "hare.h"

int test_func(const wchar_t * lol) {
  LOGGING_FUNC_START;

  INFO(L"Opana %s", L"__Ne ponyal na");
  MEMORY(L"Lol is: %s", lol);
  IO(L"IO-shechki bibib %d %d",12, 13);
  ERROR(L"A ya eby gusei");
  WARNING(L"op %s %s %s",L"musorok",L"!",L"Ne shei mne srok");

  LOGGING_FUNC_STOP;
  return 0;
}

int test_func2() {
  LOGGING_FUNC_START;
  MEMORY(L"GET THE FUCKING FLOWERS!");
  LOGGING_FUNC_STOP;
  return 0;
}
