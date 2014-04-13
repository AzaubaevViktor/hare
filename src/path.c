#include "path.h"

/* Convert `path` to canonical view. Return _new_ string */
char *pathToCanon(char *path) {
    //char *current = path;

    int64_t i = strlen(path);
    char *current = (char *)calloc(i + 1, sizeof(char));
    strcpy(current, path);
    if (*(current + 0) != '.' && *(current + 1) != '/'){
        if ((current = realloc(current, (i + 3) * sizeof(char))) == NULL){
                return NULL;
        }
        memmove(current + 2, current, (i + 1) * sizeof(char));
        *(current + 0) = '.';
        *(current + 1) = '/';
    }
    if (*(current + strlen(current) - 1) == '/'){
        i = strlen(current);
        if ((current = (char *)realloc(current, (i + 2) * sizeof(char))) == NULL){
            return NULL;
    }
        *(current + i) = '.';
        *(current + i + 1) = '\0';
    }
    return current;
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
    int64_t len = strlen(pathCan);
        int64_t i, j = 0;
        for (i = 0; i < len; i++){
            if (*(pathCan + i) == '/') j++;
        }
        return j - 1;
}

/* return file name to extract by path in current directory*/
char *getFileByPath(char *pathCan, char *dest) {
  return NULL;
}

/* Folder or not folder (using after getFileByPath) */
int isFolder(char *pathCan) {
    int64_t len = strlen(pathCan);
    if (*(pathCan + len - 2) == '/' && *(pathCan + len - 1) == '.') return 1;
    else return 0;
}
