#include "mlog.h"
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

int g_showLog = true;

void log_chars_hex(const char *pChars, int length) {
    char *str = (char *) malloc(sizeof(char) * length * 3 + 1);
    str[0] = '\0';
    char temp[4] = {'\0'};
    for (size_t i = 0; i < length; ++i) {
//        printf(" %02hhx", (unsigned char)(*(chars + i)));
        snprintf(temp, 4, " %02hhx", (unsigned char) (*(pChars + i)));
//        printf(" %s",temp);
        strcat(str, temp);
    }
    LOGD("%s",str);
//    printf("%s\n", str);
    free(str);
//    for (int i = 0; i < len && *pChars != '\0'; ++pChars, i++) {
//        LOGD("%02x", *pChars);
//    }
}