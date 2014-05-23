#include "concat_string.h"

char* concatenateStrings(const char * str1, const char * str2)
{
    size_t lengthStr1 = strlen(str1);
    size_t i;
    char * result = (char*)calloc((strlen(str1) + strlen(str2) + 1), sizeof(char));

    if (NULL == result)
    {
        return NULL;
    }

    for (i = 0; i < lengthStr1; i++)
    {
        result[i] = str1[i];
    }
    for (i = 0; i < strlen(str2); i++)
    {
        result[i + lengthStr1] = str2[i];
    }

    return result;
}
