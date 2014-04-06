#ifndef PATH_H
#define PATH_H

#include <stdlib.h>

/* Convert `path` to canonical view. Return _new_ string */
char *pathToCanon(char *path) {
  return NULL;
}

/* If `pathCan` in begin of `destCan` `pathInDest` return 1 */
int pathInDest(char *pathCan, char *destCan) {
  return 0;
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

#endif // PATH_H
