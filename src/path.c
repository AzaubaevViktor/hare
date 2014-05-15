#include "path.h"

/* Convert `path` to canonical view. Return _new_ string */
char *pathToCanon(char *path) {
  //char *current = path;

  int64_t i = strlen(path);
  char *current = (char *)calloc(i + 1, sizeof(char));
  strcpy(current, path);
  if (*(current + 0) == '/'){
    if ((current = realloc(current, (i + 2) * sizeof(char))) == NULL){
      return NULL;
    }
    memmove(current + 1, current, (i + 1) * sizeof(char));
    *(current + 0) = '.';
  }
  else if ((*(current + 0) != '.'&& *(current + 1) != '/') || (*(current + 0) != '.'&& *(current + 1) == '/')){
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
  for (pos = 0; pos <= minLen; pos++) {

    if (0 == pathCan[pos]) {
      return 1;
    }

    if (('.' == pathCan[pos]) && (0 == pathCan[pos+1])) {
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


/* return file name to extract by path in current directory, use only after pathInDest!*/
char *getFileByPath(char *path, char *dest) {
  int64_t lenPath = strlen(path);
  int64_t lenDest = strlen(dest);
  char *name = NULL;
  int64_t pos = 0;

  if ('.' == path[lenPath]) {
    name = calloc((lenDest-lenPath) + 2, sizeof(char));
    name[0] = '.';
    name[1] = '/';
    strcpy(name + 2, dest + (lenDest - lenPath - 1));
    return name;
  } else {
    if (lenDest == lenPath) {
      for (pos=lenDest; 0 < pos; pos--) {
        if ('/' == dest[pos]) {
          name = calloc(lenDest-pos + 1,sizeof(char));
          name[0] = '.';
          strcpy(name+1,dest+pos);
          return name;
        }
      }
    } else {
      name = calloc((lenDest - lenPath) + 1, sizeof(char));
      name[0] = '.';
      if ((levels(path) < levels(dest)) && (!isFolder(path)))  {
        strcpy(name + 1, dest + lenPath);
      } else {
        strcpy(name + 1, dest + (lenPath - 2));
      }
      return name;
    }
  }

  return NULL;
}


/* Folder or not folder (using after getFileByPath) */
int isFolder(char *pathCan) {
  int64_t len = strlen(pathCan);
  if (*(pathCan + len - 2) == '/' && *(pathCan + len - 1) == '.') return 1;
  else return 0;
}


char *getLastName(char *path) {
  int64_t i = 0;
  char *name = NULL;
  for (i=strlen(path)-isFolder(path)*2-1; (i>=0) && ('/' != path[i]); i--);
  if (isFolder(path)) {
    i+=1;
    name = calloc(strlen(path)-i-2, sizeof(char));
    memcpy(name,path+i,strlen(path)-i-2);
  } else {
    i+=1;
    name = calloc(strlen(path)-i, sizeof(char));
    memcpy(name,path+i,strlen(path)-i);
  }
  return name;
}


char *getFileByPathWithFolder(char *path, char *dest) {
  char *buf = NULL;
  char *lastName = NULL;
  char *name = NULL;
  buf = getFileByPath(path,dest);

  if (levels(path) < levels(dest)) {
    lastName = getLastName(path);
    name = calloc(strlen(lastName) + 1 + strlen(buf), sizeof(char));
    strcpy(name, "./");
    strcpy(name + strlen(name), lastName);
    strcpy(name + strlen(name), buf + 1);
    free(lastName);
  } else {
    name = calloc(strlen(buf), sizeof(char));
    return buf;
  }
  return name;
}
