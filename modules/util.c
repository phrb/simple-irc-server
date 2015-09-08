#include "../include/util.h"

char *stradd(char *str1, char *str2) {
    char *result = malloc(strlen(str1)+strlen(str2)+1);
    strcpy(result, str1);
    strcat(result, str2);
    return result;    
};

char *strset(char *str) {
    char *result = malloc(strlen(str)+1);
    strcpy(result, str);
    return result;    
};
