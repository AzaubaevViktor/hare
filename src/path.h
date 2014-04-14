#ifndef PATH_H
#define PATH_H

#include "logging.h"
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#define min(a,b) (((a) < (b)) ? (a) : (b))

/* Convert `path` to canonical view. Return _new_ string */
char *pathToCanon(char *path);

/* If `pathCan` in begin of `destCan` `pathInDest` return 1 */
int pathInDest(char *pathCan, char *destCan);

/* return nesting level */
int levels(char *pathCan);

/* return file name to extract by path in current directory*/
char *getFileByPath(char *pathCan, char *dest);

/* Folder or not folder (using after getFileByPath) */
int isFolder(char *pathCan);

#endif // PATH_H
