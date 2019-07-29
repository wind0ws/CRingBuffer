#include "logger_android.h"
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

int g_showLog = true;

void log_chars_hex(const char *pChars, int length) {
    char str[sizeof(char) * length * 3 + 1];
    str[0] = '\0';
    char temp[4] = {'\0'};
    for (size_t i = 0; i < length; ++i) {
        snprintf(temp, 4, " %02hhx", (unsigned char) (*(pChars + i)));
        strcat(str, temp);
    }
    LOGD("%s",str);
}