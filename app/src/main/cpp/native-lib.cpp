#include <jni.h>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include "mlog.h"
#include "ring_msg_queue.h"

extern "C" JNIEXPORT jstring JNICALL
Java_tv_yuyin_nativeapp_jni_NativeLibJni_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_tv_yuyin_nativeapp_jni_NativeLibJni_setLoggable(JNIEnv *env,
                                                     jclass type,
                                                     jboolean isLoggable) {
    g_showLog = isLoggable;
    ALOGV("JNITag", "testLog");
}

extern "C" JNIEXPORT jbyteArray JNICALL
Java_tv_yuyin_nativeapp_jni_NativeLibJni_stringBytesFromJni(
        JNIEnv *env,
        jclass type,
        jstring content) {
    LOGD("enter %s:%d", __func__, __LINE__)
    const char *chars = env->GetStringUTFChars(content, NULL);

    int strLenOfChars = (int) strlen(chars);
    jsize jstringLen = env->GetStringLength(content);
    LOGD("chars:%s, strLenOfChars=%d, jstringLen=%d", chars, strLenOfChars, jstringLen)
    jbyteArray stringjbytes = env->NewByteArray(strLenOfChars);
    env->SetByteArrayRegion(stringjbytes, 0, strLenOfChars, reinterpret_cast<const jbyte *>(chars));

    env->ReleaseStringUTFChars(content, chars);
    return stringjbytes;
}

typedef struct {
    void *msg_queue_handle;
    bool *flag_exit;
} thread_context_t;

static void *thread_fun_producer(void *thread_context) {
    thread_context_t *context = (thread_context_t *) thread_context;
    void *msgQueue = context->msg_queue_handle;
    QueueMsg ringQueueMsg = { 0 };

    for (int i = 0; i < 60; ++i) {
        ringQueueMsg.what = i;
        MSG_OBJ_DATA_TYPE *data = ringQueueMsg.obj.data;
        memset(data, 0, 1024);
        snprintf(data, 1024, "Hello World. Ring Queue Message at %d", i);
        ringQueueMsg.obj.data_len = strlen(data) + 1;
        ringQueueMsg.arg1 = i * 100;
        ringQueueMsg.arg2 = i * 200;

        push_msg:
        if (!RingMsgQueue_is_full(msgQueue)) {
            RingMsgQueue_push(msgQueue, &ringQueueMsg);
        } else {
            LOGW("producer: MsgQueue is full, try again after 150ms");
            usleep(150000);
            goto push_msg;
        }
        LOGD("producer produce=>%s", data);
        usleep(100000);
    }

    LOGE("producer is finished");
    return NULL;
}

static void *thread_fun_consumer(void *thread_context) {
    thread_context_t *context = (thread_context_t *) thread_context;
    void *msgQueue = context->msg_queue_handle;
    bool *flag_exit = context->flag_exit;
    QueueMsg ringQueueMsg = { '\0' };

    while (!(*flag_exit)) {
        if (RingMsgQueue_is_empty(msgQueue)) {
            LOGW("consumer: MsgQueue is empty,try again after 200ms");
            usleep(200000);
            continue;
        }
        if (RingMsgQueue_pop(msgQueue, &ringQueueMsg)) {
            MSG_OBJ_DATA_TYPE *data = ringQueueMsg.obj.data;
            int dataLen = ringQueueMsg.obj.data_len;
            LOGI("popped out msg. what=%d,arg1=%d,arg2=%d,obj_len=%d,obj=%s",
                 ringQueueMsg.what, ringQueueMsg.arg1, ringQueueMsg.arg2, dataLen, data);
            usleep(200000);
        } else {
            LOGW("RingMsgQueue pop failed");
        }
    }
    LOGE("consumer is finished");
    return NULL;
}


extern "C" JNIEXPORT void JNICALL
Java_tv_yuyin_nativeapp_jni_NativeLibJni_testRingMsgQueue(JNIEnv *env, jclass type) {
    void *msgQueue = RingMsgQueue_create(10);
    LOGI("now created msg queue. available_msg=%d, remain_space=%d",
         RingMsgQueue_available_pop_msg_size(msgQueue),
         RingMsgQueue_available_push_msg_size(msgQueue));

    bool flag_exit_thread = false;
    thread_context_t thread_context = {
            .msg_queue_handle = msgQueue,
            .flag_exit = &flag_exit_thread
    };
    pthread_t producer, consumer;
    pthread_create(&producer, NULL, thread_fun_producer, &thread_context);
    pthread_create(&consumer, NULL, thread_fun_consumer, &thread_context);

    LOGD("--> now join producer thread");
    pthread_join(producer, NULL);
    LOGD("<-- join producer thread finished");

    LOGI("producer is finished. available_msg=%d, remain_space=%d",
         RingMsgQueue_available_pop_msg_size(msgQueue),
         RingMsgQueue_available_push_msg_size(msgQueue));

    sleep(10);
    flag_exit_thread = true;

    LOGD("--> now join consumer thread");
    pthread_join(consumer, NULL);
    LOGD("<-- join consumer thread finished");

    LOGD("consumer is finished. now available_pop_msg=%d, available_pop_msg=%d",
         RingMsgQueue_available_pop_msg_size(msgQueue),
         RingMsgQueue_available_push_msg_size(msgQueue));

    RingMsgQueue_destroy(msgQueue);
    msgQueue = NULL;
    LOGE("destroyed msg queue");
}