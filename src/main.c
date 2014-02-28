#include <stdio.h>

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

