#ifndef DELETEFILEBLOCK_H
#define DELETEFILEBLOCK_H

#include <stdio.h>
#include <inttypes.h>
#include "lowfile.h"

void delBlock(const char * fileName, uint64_t offset, uint64_t origin);

#endif // DELETEFILEBLOCK_H
