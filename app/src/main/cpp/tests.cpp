#include <jni.h>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include "logger_android.h"
#include "ring_msg_queue.h"
#include "msg_queue_handler.h"
#include "simple_queue.h"

extern "C" JNIEXPORT jstring JNICALL
Java_tv_yuyin_nativeapp_jni_NativeTestsJni_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_tv_yuyin_nativeapp_jni_NativeTestsJni_setLoggable(JNIEnv *env,
                                                     jclass type,
                                                     jboolean isLoggable) {
    g_showLog = isLoggable;
    ALOGV("JNITag", "testLog");
}

extern "C" JNIEXPORT jbyteArray JNICALL
Java_tv_yuyin_nativeapp_jni_NativeTestsJni_stringBytesFromJni(
        JNIEnv *env,
        jclass type,
        jstring content) {
    LOGD("enter %s:%d", __func__, __LINE__)
    const char *chars = env->GetStringUTFChars(content, nullptr);

    int strLenOfChars = (int) strlen(chars);
    jsize jstringLen = env->GetStringLength(content);
    LOGD("chars:%s, strLenOfChars=%d, jstringLen=%d", chars, strLenOfChars, jstringLen)
    jbyteArray stringjbytes = env->NewByteArray(strLenOfChars);
    env->SetByteArrayRegion(stringjbytes, 0, strLenOfChars, reinterpret_cast<const jbyte *>(chars));

    env->ReleaseStringUTFChars(content, chars);
    return stringjbytes;
}

typedef struct {
    ring_msg_queue msg_queue_handle;
    bool *flag_exit;
} thread_context_t;

static void *thread_fun_producer(void *thread_context) {
    thread_context_t *context = (thread_context_t *) thread_context;
    ring_msg_queue msgQueue = context->msg_queue_handle;
    queue_msg_t ringQueueMsg = {0};

    for (int i = 0; i < 60; ++i) {
        ringQueueMsg.what = i;
        MSG_OBJ_DATA_TYPE *data = ringQueueMsg.obj.data;
        memset(data, 0, MSG_OBJ_MAX_CAPACITY);
        snprintf(data, MSG_OBJ_MAX_CAPACITY, "Hello World. Ring Queue Message at %d", i);
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
    return nullptr;
}

static void *thread_fun_consumer(void *thread_context) {
    thread_context_t *context = (thread_context_t *) thread_context;
    ring_msg_queue msgQueue = context->msg_queue_handle;
    bool *flag_exit = context->flag_exit;
    queue_msg_t ringQueueMsg = {'\0'};

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
    return nullptr;
}


extern "C" JNIEXPORT void JNICALL
Java_tv_yuyin_nativeapp_jni_NativeTestsJni_testRingMsgQueue(JNIEnv *env, jclass type) {
    ring_msg_queue msgQueue = RingMsgQueue_create(sizeof(queue_msg_t), 10);
    LOGI("now created msg queue. available_msg=%d, remain_space=%d",
         RingMsgQueue_available_pop_msg_amount(msgQueue),
         RingMsgQueue_available_push_msg_amount(msgQueue));

    bool flag_exit_thread = false;
    thread_context_t thread_context = {
            .msg_queue_handle = msgQueue,
            .flag_exit = &flag_exit_thread
    };
    pthread_t producer, consumer;
    pthread_create(&producer, nullptr, thread_fun_producer, &thread_context);
    pthread_create(&consumer, nullptr, thread_fun_consumer, &thread_context);

    LOGD("--> now join producer thread");
    pthread_join(producer, nullptr);
    LOGD("<-- join producer thread finished");

    LOGI("producer is finished. available_msg=%d, remain_space=%d",
         RingMsgQueue_available_pop_msg_amount(msgQueue),
         RingMsgQueue_available_push_msg_amount(msgQueue));

    sleep(10);
    flag_exit_thread = true;

    LOGD("--> now join consumer thread");
    pthread_join(consumer, nullptr);
    LOGD("<-- join consumer thread finished");

    LOGD("consumer is finished. now available_pop_msg=%d, available_pop_msg=%d",
         RingMsgQueue_available_pop_msg_amount(msgQueue),
         RingMsgQueue_available_push_msg_amount(msgQueue));

    RingMsgQueue_destroy(msgQueue);
    msgQueue = nullptr;
    LOGE("destroyed msg queue");
}

static void queue_handle_msg(queue_msg_t *msg_p) {
    int data_len = msg_p->obj.data_len;
    MSG_OBJ_DATA_TYPE *data = msg_p->obj.data;
    switch (msg_p->what) {
        case 0:
            LOGI("case 0. arg1=%d,arg2=%d,data_len=%d,data=%s",
                 msg_p->arg1, msg_p->arg2, data_len, data);
            break;
        case 1:
            LOGI("case 1. arg1=%d,arg2=%d,data_len=%d,data=%s",
                 msg_p->arg1, msg_p->arg2, data_len, data);
            break;
        default:
            LOGW("default branch. what=%d,arg1=%d,arg2=%d,data_len=%d,data=%s",
                 msg_p->what, msg_p->arg1, msg_p->arg2, data_len, data);
            break;
    }
    //mock time consuming operation
    usleep(300000);
}

extern "C" JNIEXPORT void JNICALL
Java_tv_yuyin_nativeapp_jni_NativeTestsJni_testQueueHandler(JNIEnv *env, jclass type) {
    queue_handler handler_p = QueueHandler_create(10, queue_handle_msg);

    queue_msg_t msg = {0};
    for (int i = 0; i < 50; ++i) {
        msg.what = i % 10;
        msg.arg1 = msg.what * 100;
        msg.arg2 = msg.what * 200;
        snprintf(msg.obj.data, MSG_OBJ_MAX_CAPACITY, "Hello Msg Queue Handler at %d", i);
        msg.obj.data_len = strlen(msg.obj.data) + 1;
        send_msg:
        if (!QueueHandler_send(handler_p, &msg)) {
            QueueHandler_clear(handler_p);
            LOGE("queue is full, clear it finished");
            usleep(150000);
//            LOGE("queue is full, retry after 150ms");
//            usleep(150000);
            goto send_msg;
        }
        LOGD("pushed msg: %s", msg.obj.data);
        //mock send msg interval
        usleep(100000);
    }
    LOGE("push msg completed!");

    sleep(8);
    QueueHandler_destroy(handler_p);
    handler_p = nullptr;
    LOGE("destroyed msg queue handler");
}

extern "C" JNIEXPORT void JNICALL
Java_tv_yuyin_nativeapp_jni_NativeTestsJni_testSemaphore(JNIEnv *env, jclass type) {
    LOGD("enter %s:%d", __func__, __LINE__);
    sem_t sem;
    sem_init(&sem, 0, 0);
    sem_destroy(&sem);
    sem_init(&sem, 0, 0);
    sem_destroy(&sem);
    LOGI("test semaphore succeed");
}

extern "C" JNIEXPORT void JNICALL
Java_tv_yuyin_nativeapp_jni_NativeTestsJni_testSimpleQueue(JNIEnv *env, jclass type) {
    LOGD("enter %s:%d", __func__, __LINE__);
    simple_queue_handle handle = simple_queue_init();
    LOGI("after init. simple_queue_available_space=%d, simple_queue_available_data=%d",
         simple_queue_available_space(handle), simple_queue_available_data(handle));
    LOGI("simple_queue_is_full=>%d, simple_queue_is_empty=%d",
         simple_queue_is_full(handle), simple_queue_is_empty(handle));
    if (simple_queue_push(handle, 'A') == SIMPLE_QUEUE_SUCCESS) {
        LOGD("succeed push 'A' to queue");
    }
    if (simple_queue_push(handle, 'B') == SIMPLE_QUEUE_SUCCESS) {
        LOGD("succeed push 'B' to queue");
    }
    LOGI("simple_queue_is_full=>%d, simple_queue_is_empty=%d",
         simple_queue_is_full(handle), simple_queue_is_empty(handle));
    LOGI("after push two char. simple_queue_available_space=%d, simple_queue_available_data=%d",
         simple_queue_available_space(handle), simple_queue_available_data(handle));
    char data = '\0';
    if (simple_queue_pop(handle, &data) == SIMPLE_QUEUE_SUCCESS) {
        LOGD("succeed popped out data=%c", data);
    }
    if (simple_queue_pop(handle, &data) == SIMPLE_QUEUE_SUCCESS) {
        LOGD("succeed popped out data=%c", data);
    }
    LOGI("after poped two char. simple_queue_available_space=%d, simple_queue_available_data=%d",
         simple_queue_available_space(handle), simple_queue_available_data(handle));
    LOGI("simple_queue_is_full=>%d, simple_queue_is_empty=%d", simple_queue_is_full(handle),
         simple_queue_is_empty(handle));
    if (simple_queue_pop(handle, &data) == SIMPLE_QUEUE_FAIL) {
        LOGE("no data to pop. queue is empty");
    }
    simple_queue_destroy(&handle);
    LOGI("test queue succeed");
}







//===========================ring_buf test case=====================================================
#include "ring_buf.h"
typedef struct {
    int i;
    short s;
    char c;
}ring_data_t;


typedef struct {
    ring_handle handle;
    bool flag_exit;
}ring_buf_thread_context_t;

static void *ring_buf_producer_thread(void *param){
    ring_buf_thread_context_t *context = (ring_buf_thread_context_t *)param;
//    SimpleQueueDataType data = 'A';
    char data = 'A';
    while (!context->flag_exit){
        if(data > 'Z'){
            data = 'A';
        }
        if (ring_buf_write(context->handle, &data, sizeof(data))){
            LOGD("push %c \n", data);
            data++;
            usleep(20000);
        } else {
            usleep(40000);
        }
    }
    LOGD("=== exit producer ===\n");
    return NULL;
}

static void *ring_buf_consumer_thread(void *param){
    ring_buf_thread_context_t *context = (ring_buf_thread_context_t *)param;
//    SimpleQueueDataType data = 0;
    char data[5] = {'0'};
    while (1){
        if(ring_buf_read(context->handle, &data, 4)){
            LOGD("              pop %s\n",data);
            usleep(80000);
        } else if(context->flag_exit){
            uint32_t last_data = ring_buf_available_data(context->handle);
            if (last_data > 0) {
                memset(data, '0', 5);
                ring_buf_read(context->handle, &data, last_data);
                LOGD("           last packet[%d] data=>%s\n",last_data, data);
            }
            break;
        } else{
            usleep(100000);
        }
    }
    LOGD("=== exit consumer ===\n");
    return NULL;
}


extern "C" JNIEXPORT void JNICALL
Java_tv_yuyin_nativeapp_jni_NativeTestsJni_testRingBuf(JNIEnv *env, jclass type) {

    ring_handle handle = ring_buf_create(12 * sizeof(ring_data_t));
    LOGD("after ring create, data_amount=%d, available_space=%d\n", ring_buf_available_data(handle),
           ring_buf_available_space(handle));


    ring_buf_thread_context_t threadContext = {
            .handle = handle,
            .flag_exit = false
    };
    pthread_t pthread_producer;
    pthread_create(&pthread_producer,NULL,ring_buf_producer_thread,&threadContext);
    pthread_t pthread_consumer;
    pthread_create(&pthread_consumer,NULL,ring_buf_consumer_thread,&threadContext);

    sleep(8);

    threadContext.flag_exit = true;
    pthread_join(pthread_producer,NULL);
    pthread_join(pthread_consumer,NULL);
    LOGD("after pop done, data_amount=%d, available_space=%d\n", ring_buf_available_data(handle),
           ring_buf_available_space(handle));


    int counter = 0;
    while (counter < 10) {
        LOGD("\n\n\n===========[%d]===========\n", counter++);
        ring_data_t msg = {0};
        for (char c = 'A'; c < 'A' + 12; ++c) {
            msg.c = c;
            msg.s = c;
            msg.i = c;
            LOGD("push %c result %d\n", c, ring_buf_write(handle, &msg, sizeof(msg)));
        }
        LOGD("after ring push, data_amount=%d, available_space=%d\n", ring_buf_available_data(handle),
               ring_buf_available_space(handle));

        for (int i = 0; i < 12; ++i) {
            if (ring_buf_read(handle, &msg, sizeof(msg))) {
                LOGD("pop %d times succeed, result=%c\n", i, msg.c);
            } else {
                LOGD("pop failed at %d\n", i);
            }
        }
        LOGD("after ring pop, data_amount=%d, available_space=%d\n", ring_buf_available_data(handle),
               ring_buf_available_space(handle));
    }

    ring_buf_destroy(&handle);


}