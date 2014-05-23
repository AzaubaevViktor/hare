#include "continue_func.h"

int continue_func(){

    unsigned char c1;
    int result;
    printf("Хотите продолжить [Y/n]:");
    c1 = fgetc(stdin);
    //if (c1 == '\n') c1 = fgetc(stdin);
    if (c1 == 'n' || c1 == 'N') result = STOP;
    else if (c1 == 'y' || c1 == 'Y' || c1 == '\n') result = NEXT;
    else result = ITER;
    while(c1 != '\n'){
        c1 = fgetc(stdin);
    }
    if (result == ITER) return continue_func(); else
        return result;
}
