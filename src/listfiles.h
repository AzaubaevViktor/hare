#ifndef LISTFILES_H
#define LISTFILES_H
#include "logging.h"
#include "archfiles.h"
#include "path.h"
#include "findsign.h"
#include "continue_func.h"
#include "hare.h"

#define READ_ARCH_ERROR (3321)
#define SIZE_BLOCK (16)
#define ABORT_LISTING (96)

int _printFilesOfFolder(FILE *fileArch, char *nameFolder);
#endif // LISTFILES_H
