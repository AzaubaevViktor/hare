#include "logging.h"
#include "archfiles.h"
#include <stdio.h>
#include <time.h>

GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
  INIT_LOGGING;

  FILE *f = fopen("test", "wb");
  FileInfo fInfo, fInfo1;
  ArchFileInfo info, info1;
  long int i;

  fInfo.name = "info.txt";
  fInfo.sizeName = 8;
  fInfo.timeLastModification = 10007463880;
  fInfo.size = 1025;

  info.fileInfo = &fInfo;

  info.dataSize = 1025;
  info.endUnusedBits = 0;
  info.flags = 0;
  info.haffTreeSize = 10;
  info.haffTree = "This is ha";
  info.HeaderCheckSum = 1;


  writeFileHeader(f, &info, 1);

  fclose(f);

  info1.fileInfo = &fInfo1;

  f = fopen("test","rb");

  readHeader(f, &info1);

  printf("%s: size:%d\n",info1.fileInfo->name, info1.dataSize);

  fclose(f);

  DEINIT_LOGGING;
  printf("Programm exit!\n");
  return 0;
}

