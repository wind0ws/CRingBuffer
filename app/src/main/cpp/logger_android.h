#ifndef MLOG_H_
#define MLOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <android/log.h>

#define LOG_TAG "NativeJni"

extern int g_showLog;

#define ALOGV(TAG, ...) if(g_showLog) {\
__android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__);\
}
#define ALOGD(TAG, ...) if(g_showLog) {\
__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__);\
}
#define ALOGI(TAG, ...) if(g_showLog) {\
__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__);\
}
#define ALOGW(TAG, ...) if(g_showLog) {\
__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__);\
}
#define ALOGE(TAG, ...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__);

#define LOGV(...) if(g_showLog) {\
__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__);\
}
#define LOGD(...) if(g_showLog) {\
__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);\
}
#define LOGI(...) if(g_showLog) {\
__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);\
}
#define LOGW(...) if(g_showLog) {\
__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__);\
}
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);
//#define LOGV(...) do{if(g_showLog) {__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__);}}while(0);
//#define LOGD(...) do{if(g_showLog) {__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);}}while(0);
//#define LOGI(...) do{if(g_showLog) {__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);}}while(0);
//#define LOGW(...) do{if(g_showLog) {__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__);}}while(0);
//#define LOGE(...) do{__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);}while(0);

void log_chars_hex(const char *chars, int length);

#ifdef __cplusplus
}
#endif

#endif /* MLOG_H_ */
