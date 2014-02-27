#include <stdio.h>

#include "fileinfo.h"

int main(int argc, char *argv[], char *env[])
{
    FileInfo fi;

    getFileInfo("text.txt", &fi);
    printFileInfo(fi);

  printf("Hello World!\n");
  return 0;
}

