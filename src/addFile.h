#ifndef ADDFILE_H
#define ADDFILE_H

#include "archfiles.h"
#include "concat_string.h"
#include "errors.h"
#include "lowfile.h"
#include "hare.h"
#include "huff.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>


#define SIZE_BLOCK (1000000)



/* если recurse == 1, то начинает рекурсивно обходить папки и добавлять файлы из них. иначе пнх! */
void addFiles2Arch(Context context);
int addFile2Arch(ArchFileInfo archFileInfo, const char* nameArchive);
int writeFolderHeader(Context context, const char * folderName);
void recurseAddFiles2Arch(char * path, Context context);

#endif // ADDFILE_H
