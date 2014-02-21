#ifndef HARE_H
#define HARE_H
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "fileinfo.h"

typedef struct _FileNode {
  char *fileName;
  FILE *file;
  struct FileInfo *fileInfo;
  struct _FileNode *next;
} FileNode;

typedef struct {
  int argc;
  char **argv;
  char **env;
  int8_t keys; /* 0x80,0x40, ... , 0x2, 0x1: Ø, Ø, Ø, Add, eXtract, Delete, List, inTegrity check */
  char *archName;
  char **workFiles; /* Передали через аргументы */
  FileNode *filesTree;
} Context;



int parseArgs(Context *cnt);

int getFilesInfo(Context *cnt);
int getArchFileInfo(Context *cnt, int64_t offset, struct FileInfo *info);

int addFiles2Arch(Context *cnt, struct FileInfo *files);

int findArchFile(Context *cnt, char *fileName);

int checkIntegrity(Context *cnt);

int getArchFiles(Context *cnt);
int printArchFiles(Context *cnt);

int extractFiles(Context *cnt, struct FileInfo *files);

int deleteFileBlock(Context *cnt, int64_t offset, int64_t length);
int deleteArchFiles(Context *cnt, struct FileInfo *files);

#endif // HARE_H
