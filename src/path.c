#include "path.h"

/* Convert `path` to canonical view. Return _new_ string */
char *pathToCanon(char *path) {
  return NULL;
}

/* If `pathCan` in begin of `destCan` `pathInDest` return 1 */
int pathInDest(char *pathCan, char *destCan) {
  int64_t pos = 0;
  int64_t minLen = min(strlen(pathCan), strlen(destCan));
  for (pos = 2; pos < minLen; pos++) {
    if (('.' == pathCan[pos]) && (0 == pathCan[pos])) {
      return 1;
    }
    if (pathCan[pos] != destCan[pos]) {
      return 0;
    }
  }

  return 1;
}

/* return nesting level */
int levels(char *pathCan) {
  return 0;
}

/* return file name to extract by path in current directory*/
char *getFileByPath(char *pathCan, char *dest) {
  return NULL;
}

/* Folder or not folder (using after getFileByPath) */
int isFolder(char *pathCan) {
  return 0;
}
