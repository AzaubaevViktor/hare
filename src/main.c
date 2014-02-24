#include "init.h"

int main(int argc, char *argv[], char *env[])
{
    Context *cnt = parseArgs(argc, argv, env);
    printf("%s\n", *(cnt->workFiles));
  	return 0;
}

