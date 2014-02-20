#ifndef HARE_H
#define HARE_H
#include <inttypes.h>
#include <stdint.h>

typedef struct {
  ;
} FileInfo;

typedef struct {
  ;
} FileNode;

typedef struct {
  int argc;
  char *argv[];
  char *env[];
  int8_t keys; /* 0x80,0x40, ... , 0x2, 0x1: Ø, Ø, Ø, Add, eXtract, Delete, List, inTegrity check */
  char *archName;
  char *workFiles[]; /* Передали через аргументы */
  FileNode *filesTree;
} Context;



int parseArgs(Context *cnt);

int getFilesInfo(Context *cnt);
int getArchFileInfo(Context *cnt, int64_t offset, FileInfo *info);

int addFiles2Arch(Context *cnt, FileInfo *files);

int findArchFile(Context *cnt, char *fileName);

int checkIntegrity(Context *cnt);

int getArchFiles(Context *cnt);
int printArchFiles(Context *cnt);

int extractFiles(Context *cnt, FileInfo *files);

int deleteFileBlock(Context *cnt, int64_t offset, int64_t length);
int deleteArchFiles(Context *cnt, FileInfo *files);

#endif // HARE_H