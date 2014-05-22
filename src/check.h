#ifndef CHECK_H
#define CHECK_H
#include "lowfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "findsign.h"
#include "archfiles.h"

int checkIntegrity_my(FILE *arch);
int checkingHeader(FILE *arch, ArchFileInfo *info, int64_t *real);
int checkingData(FILE *arch, ArchFileInfo *info, int64_t *real);
#endif // CHECK_H
