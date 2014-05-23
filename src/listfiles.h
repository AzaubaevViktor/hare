#ifndef LISTFILES_H
#define LISTFILES_H
#include "logging.h"
#include "archfiles.h"
#include "path.h"
#include "findsign.h"
#define READ_ARCH_ERROR (3321)
#define SIZE_BLOCK (16)

int printFilesOfFolder(FILE *fileArch, char *nameFolder);
#endif // LISTFILES_H
