#include "hare.h"

int test_func(const char * lol) {
  INFO("Opana %s", "__Ne ponyal na");
  MEMORY("Lol is: %s", lol);
  IO("IO-shechki bibib %d %d",12, 13);
  ERROR("A ya eby gusei");
  WARNING("op %s %s %s","musorok","!","Ne shei mne srok");

  return 0;
}
