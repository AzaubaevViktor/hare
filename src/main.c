#include "hare.h"
#include "helper.h"
GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
    Context *cnt = (Context *)malloc(sizeof(Context));
    INIT_LOGGING
    cnt->argv = argv;
    cnt->argc = argc;
    cnt->env  = env;
    parseArgs(&cnt);
    printHelp();
    INFO("Прочитали строку из аргументов маздайным образом: %s", *(cnt->workFiles))
    DEINIT_LOGGING
  	return 0;
}

