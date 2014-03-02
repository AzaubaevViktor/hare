#include "init.h"

void printHelp(){
    FILE *file_help;
    if ((file_help = fopen("../src/help.txt", "r")) == NULL){
        printf("NO!\n");
    }
    fseek(file_help,0,SEEK_END);
    long size = ftell(file_help);
    fseek(file_help, 0, SEEK_SET);
    char *str = (char *)malloc(size);
    fread(str, size, 1, file_help);
    printf("%s\n", str);
    free(str);
}
