#include "hare.h"

int test_func(const char * lol) {
  LOGGING_FUNC_START;

  INFO("Opana %s", "__Ne ponyal na");
  MEMORY("Lol is: %s", lol);
  IO("IO-shechki bibib %d %d",12, 13);
  ERROR("A ya eby gusei");
  WARNING("op %s %s %s","musorok","!","Ne shei mne srok");

  LOGGING_FUNC_STOP;
  return 0;
}

int test_func2() {
  LOGGING_FUNC_START;
  MEMORY("GET THE FUCKING FLOWERS!");
  LOGGING_FUNC_STOP;
  return 0;
}
