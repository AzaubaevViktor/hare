
/* by Alex Mukhin */
#include "hare.h"
#include "logging.h"
#include "addfiles2arch.h"
#include <stdio.h>
#include <time.h>
GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
    Context *cnt = (Context *)malloc(sizeof(Context));
    INIT_LOGGING
    cnt->argv = argv;
    cnt->argc = argc;
    cnt->env  = env;
    parseArgs(&cnt);
    DEINIT_LOGGING
    return 0;
}

