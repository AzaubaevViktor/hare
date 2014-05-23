#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>

#define ERROR_OPEN_FILE (1234)
#define ERROR_OPEN_ARCHIVE (4321)
#define ERROR_NOT_ALLOCATED_MEMORY (1515)
#define ERROR_GET_FILE_INFO (5151)
#define ERROR_OPEN_FOLDER (1414)
#define ERROR_WRITE_IN_ARCHIVE (4141)
#define ERROR_NOT_ADD_FOLDER (9089)

#define PRINT_ERROR(error, ...) \
    {\
        switch ((error)) \
        { \
        case ERROR_GET_FILE_INFO: \
            printf("ERROR: can not get information about '%s'!\n", ## __VA_ARGS__); \
            break; \
        case ERROR_NOT_ALLOCATED_MEMORY: \
            printf("ERROR: memory was not allocated!\n"); \
            break; \
        case ERROR_OPEN_ARCHIVE: \
            printf("ERROR: can not open archive '%s'!\n", ## __VA_ARGS__); \
            break; \
        case ERROR_OPEN_FILE: \
            printf("ERROR: can not open file '%s'!\n", ## __VA_ARGS__); \
            break; \
        case ERROR_OPEN_FOLDER: \
            printf("ERROR: can not open folder '%s'!\n", ## __VA_ARGS__); \
            break; \
        case ERROR_NOT_ADD_FOLDER: \
            printf("ERROR: can not add folder '%s' in archive!\n", ## __VA_ARGS__); \
            break; \
        case ERROR_WRITE_IN_ARCHIVE: \
            printf("ERROR: can not write in archive!\n"); \
            break; \
        case 0: \
            break; \
        } \
        while (1 && error) \
        { \
            char answer = 0; \
            printf("Continue ? (y / n) \n"); \
            answer = getchar(); \
            if (('y' == answer) || ('Y' == answer)) \
                break; \
            if (('n' == answer) || ('N' == answer)) \
                exit(0); \
        } \
    }

#endif // ERRORS_H
