#ifndef LISTFILES_H
#define LISTFILES_H
#include "logging.h"
#include "archfiles.h"
#include "path.h"

#define READ_ARCH_ERROR (3321)

int printFilesOfFolder(FILE *fileArch, const char *nameFolder);
#endif // LISTFILES_H
