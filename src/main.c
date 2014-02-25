#include "init.h"

int main(int argc, char *argv[], char *env[])
{
    Context *cnt = (Context *)malloc(sizeof(Context));
    cnt->argv = argv;
    cnt->argc = argc;
    cnt->env  = env;
    parseArgs(&cnt);
    printf("%s\n", *(cnt->workFiles));
  	return 0;
}

