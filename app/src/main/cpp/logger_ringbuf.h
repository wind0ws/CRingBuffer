//
// Created by Administrator on 2019/7/26.
//

#ifndef CRINGBUFFER_LOGGER_RINGBUF_H
#define CRINGBUFFER_LOGGER_RINGBUF_H

#include "logger_android.h"

#define RING_LOGV(...) LOGV(__VA_ARGS__);
#define RING_LOGD(...) LOGD(__VA_ARGS__);
#define RING_LOGI(...) LOGI(__VA_ARGS__);
#define RING_LOGW(...) LOGW(__VA_ARGS__);
#define RING_LOGE(...) LOGE(__VA_ARGS__);

//#define RING_LOGV(...) do{LOGV(__VA_ARGS__);}while(0);
//#define RING_LOGD(...) do{LOGD(__VA_ARGS__);}while(0);
//#define RING_LOGI(...) do{LOGI(__VA_ARGS__);}while(0);
//#define RING_LOGW(...) do{LOGW(__VA_ARGS__);}while(0);
//#define RING_LOGE(...) do{LOGE(__VA_ARGS__);}while(0);

#endif //CRINGBUFFER_LOGGER_RINGBUF_H
